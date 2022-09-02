#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#define SEND_BUFF_SIZE 256
#define RECV_BUFF_SIZE 128
#define THRD_NUM 3

struct ThreadInfo {
    int socket;  // сокет, для взаимодействия с клиентом
    int num;     // номер потока(для освобождения)
} typedef ThreadInfo;

int sock;
int sock_array[THRD_NUM];
int counter = 0;

void handle_shutdown(int sig) {
    printf("\nShutdown\n");
    close(sock);
    for (int i = 0; i < THRD_NUM; i++) {
        close(sock_array[i]);
        printf("Closed %d: %d\n", i, sock_array[i]);
    }
    exit(EXIT_SUCCESS);
}

void error(char* msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

void reaper(int sig) {
    int status;
    while (wait3(&status, WNOHANG, (struct rusage*)0) >= 0)
        ;
}

/*
 * Если free_idx передаётся с положительным значением, то
 * просто устанавливаем флаг, что процесс освобожден.
 * Если free_idx отрицательно, тогда функция используется
 * для возвращения индекса следующего свободного процесса.
 */
int get_next_proc(int free_idx) {
    static int is_free[THRD_NUM] = {1, 1, 1};
    static int next_proc = 0;
    if (free_idx > -1) {
        is_free[free_idx] = 1;
        return 0;
    } else {
        for (int i = 0; i < THRD_NUM; i++) {
            next_proc = (next_proc + 1) % THRD_NUM;
            if (is_free[next_proc]) {
                is_free[next_proc] = 0;
                return next_proc;
            }
        }
    }
    return -1;
}

void* thread_work(void* input) {
    char send_buff[SEND_BUFF_SIZE];
    char recv_buff[RECV_BUFF_SIZE];
    size_t size;
    ThreadInfo* t_info = (ThreadInfo*)input;

    while (1) {
        if (t_info->socket != 0) {
            size = recv(t_info->socket, recv_buff, RECV_BUFF_SIZE, 0);
            recv_buff[size - 1] = '\0';
            sprintf(send_buff, "OK![✔][tid:%ld], MSG:[%s]\n", pthread_self(), recv_buff);
            printf("[tid:%ld]: %s\n", pthread_self(), recv_buff);
            size = send(t_info->socket, send_buff, strlen(send_buff), 0);
            if (size < 0) {
                error("send");
            }
            sleep(5);
            close(t_info->socket);
            get_next_proc(t_info->num);
            t_info->socket = 0;
        }
    }
}

int main() {
    signal(SIGINT, handle_shutdown);
    signal(SIGCHLD, reaper);
    char send_buff[SEND_BUFF_SIZE];
    char recv_buff[RECV_BUFF_SIZE];
    int new_sock;
    struct sockaddr_in server;
    size_t size;

    pid_t pid1, pid2, pid3;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        error("socket");
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = 7777;
    int reuseaddr_on = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
                   &reuseaddr_on, sizeof(reuseaddr_on)) < 0) {
        error("setsockopt");
    }

    if (bind(sock, (const struct sockaddr*)&server, sizeof(server)) < 0) {
        error("bind");
    }
    if (listen(sock, 3) < 0) {
        error("listen");
    }
    pthread_t child[THRD_NUM];
    ThreadInfo thread_info[THRD_NUM];

    int status = 0;
    for (int i = 0; i < THRD_NUM; i++) {
        thread_info[i].num = i;
        thread_info[i].socket = 0;
        status = pthread_create(&(child[i]), NULL, thread_work, &(thread_info[i]));
        if (status < 0) {
            error("pthread_create");
        }
    }

    int idx_proc;
    while (1) {
        new_sock = accept(sock, 0, 0);
        if (new_sock < 0) {
            error("accept");
        }
        idx_proc = get_next_proc(-1);
        if (idx_proc < 0) {
            printf("No free process\n");
            sprintf(send_buff, "No free process. Try again later\n");
            size = send(new_sock, send_buff, strlen(send_buff), 0);
            if (size < 0) {
                error("send");
            }
            close(new_sock);
        } else {
            sock_array[counter] = new_sock;
            thread_info[idx_proc].socket = sock_array[counter++];
        }
    }
    return 0;
}