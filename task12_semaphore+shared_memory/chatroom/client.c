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

#define SERVER_QUEUE_NAME "/eltex-homework-hello-hi"
#define QUEUE_PERIMISSIONS 0660  // r+w for owner & group
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 128
#define MSG_BUFFER_SIZE 128  // a little larger than MAX_MSG_SIZE

char sem_name[128] = "mysem1";
char shm_name_flag[128] = "/mysharedflag1";
char shm_name_msg[128] = "/mysharedmsg1";
char shm_name_sendflag[128] = "/mysharedsendflag1";
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
    exit(0);
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

    int shm_id_sendflag = shm_open(shm_name_sendflag, O_RDWR | O_CREAT, 0666);
    if (shm_id_sendflag == -1) {
        perror("shm_open_sendflag");
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
    ftrunc_ret = ftruncate(shm_id_sendflag, size);
    if (ftrunc_ret == -1) {
        perror("ftruncate_sendflag");
        exit(EXIT_FAILURE);
    }
    ftrunc_ret = ftruncate(shm_id_msg, size);
    if (ftrunc_ret == -1) {
        perror("ftruncate_msg");
        exit(EXIT_FAILURE);
    }

    addr[0] = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id_flag, 0);
    if (addr == MAP_FAILED) {
        perror("mmap_flag");
        exit(EXIT_FAILURE);
    }

    addr[1] = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id_msg, 0);
    if (addr == MAP_FAILED) {
        perror("mmap_msg");
        exit(EXIT_FAILURE);
    }
    addr[3] = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id_sendflag, 0);
    if (addr == MAP_FAILED) {
        perror("mmap_sendflag");
        exit(EXIT_FAILURE);
    }

    int* flag = (int*)addr[0];
    int* sendflag = (int*)addr[3];
    *sendflag = 0;
    *flag = 0;
    char* msg = (char*)addr[1];
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
        if (pid == 0) {
            fgets(msg_tmp, MAX_MSG_SIZE, stdin);
            sem_wait(sem);
            strcpy(msg, msg_tmp);
            *flag = 1;
            *sendflag = main_pid;
            sem_post(sem);
        } else {
            if (*flag == 1) {
                if (*sendflag != main_pid) {
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
