#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_MSG_SIZE 128

/* *
 * Переносим то, что нужно будет закрыть, в глобальную область,
 * чтобы закрывать можно было handle_shutdown
 * */

char sem_name[128] = "mysem1";
char shm_name_flag[128] = "/mysharedflag1";
char shm_name_msg[128] = "/mysharedmsg1";
char shm_name_send_pid[128] = "/mysharedsendflag1";
sem_t* sem;
void* addr[3] = {[0 ... 2] = NULL};
size_t size = MAX_MSG_SIZE;

void handle_shutdown(int sig) {
    printf("\nDisconnect %d\n", getpid());
    sem_close(sem);
    munmap(addr[0], size);
    munmap(addr[1], size);
    munmap(addr[2], size);
    sem_unlink(sem_name);
    shm_unlink(shm_name_flag);
    shm_unlink(shm_name_msg);
    shm_unlink(shm_name_send_pid);
    exit(EXIT_SUCCESS);
}

int main() {
    signal(SIGINT, handle_shutdown);
    pid_t pid;
    int main_pid = getpid();
    int shm_id_flag = shm_open(shm_name_flag, O_RDWR | O_CREAT, 0666);
    if (shm_id_flag == -1) {
        perror("shm_open_flag");
        exit(EXIT_FAILURE);
    }

    int shm_id_send_pid = shm_open(shm_name_send_pid, O_RDWR | O_CREAT, 0666);
    if (shm_id_send_pid == -1) {
        perror("shm_open_send_pid");
        exit(EXIT_FAILURE);
    }

    int shm_id_msg = shm_open(shm_name_msg, O_RDWR | O_CREAT, 0666);
    if (shm_id_msg == -1) {
        perror("shm_open_msg");
        exit(EXIT_FAILURE);
    }

    int ftrunc_ret = ftruncate(shm_id_flag, size);
    if (ftrunc_ret == -1) {
        perror("ftruncate_flag");
        exit(EXIT_FAILURE);
    }
    ftrunc_ret = ftruncate(shm_id_send_pid, size);
    if (ftrunc_ret == -1) {
        perror("ftruncate_send_pid");
        exit(EXIT_FAILURE);
    }
    ftrunc_ret = ftruncate(shm_id_msg, size);
    if (ftrunc_ret == -1) {
        perror("ftruncate_msg");
        exit(EXIT_FAILURE);
    }

    // Флаг сигнализирует о том, что в msg пришли новые данные, т.е. новое сообщение
    addr[0] = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id_flag, 0);
    if (addr == MAP_FAILED) {
        perror("mmap_flag");
        exit(EXIT_FAILURE);
    }

    // Тут храним сообщение, которое перезаписывает предыдущее
    addr[1] = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id_msg, 0);
    if (addr == MAP_FAILED) {
        perror("mmap_msg");
        exit(EXIT_FAILURE);
    }

    // send_pid хранит pid отправителя
    addr[2] = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id_send_pid, 0);
    if (addr == MAP_FAILED) {
        perror("mmap_sendflag");
        exit(EXIT_FAILURE);
    }

    int* flag = (int*)addr[0];
    int* send_pid = (int*)addr[2];
    *send_pid = 0;
    *flag = 0;
    char* msg = (char*)addr[1];

    // msg_tmp используется в fgets, чтобы вынести из критической секции(к.с.)
    // в к.с. уже копируем из msg_tmp в msg
    char msg_tmp[MAX_MSG_SIZE];

    sem = sem_open(sem_name, O_CREAT | O_RDWR, 0666, 1);
    if (sem == SEM_FAILED) {
        printf("sem_open");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    while (1) {
        /* *
         * Возможная проблема: процессы будут читать новое сообщение №1, но
         * один из клиентов написал еще одно сообщение №2 и процесс, принявший
         * это сообщение, захватил семафор и перезаписал сообщение №1, и не все
         * процессы успели прочитать его.                                        ]
         * */
        if (pid == 0) {
            fgets(msg_tmp, MAX_MSG_SIZE, stdin);
            sem_wait(sem);
            strcpy(msg, msg_tmp);
            *flag = 1;
            *send_pid = main_pid;
            sem_post(sem);
        } else {
            if (*flag == 1) {
                /* *
                 * Костыльное решение, но, пока что, лучше не придумал.
                 * Возможна такая ситуация, что первый процесс, который
                 * прошел в к.с., изменит flag на 0, а процессы, которые
                 * ещё не зашли внуть области "if (*flag == 1)", так и не
                 * получат сообщение, так как флаг уже изменен и не сообщает
                 * о новом сообщении.
                 * Поэтому здесь организуем usleep, чтобы затормозить процессы
                 * и дать всем зайти в область "if (*flag == 1)" и далее не зависеть
                 * от флага.
                 * */
                usleep(100 * 1000);  // 0.1 sec
                if (*send_pid != main_pid) {
                    /* *
                     * Не позволяем процессу отправителю пройти сюда и
                     * отправить себе СВОЁ ЖЕ сообщение
                     * */
                    sem_wait(sem);
                    *flag = 0;
                    printf("%s\n", msg);
                    sem_post(sem);
                }
            }
        }
    }

    return 0;
}
