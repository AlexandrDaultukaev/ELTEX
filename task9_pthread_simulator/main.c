#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_STORES 5
#define NUM_THREADS 4

int stores[NUM_STORES];

typedef struct _thread_data_t {
    int tid;
    int need;
} thread_data_t;

void add_goods(int idx) {
    stores[idx] += 500;
}

void* run_simulator(void* arg) {
    static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    static int store_ind;
    static int num_out_threads = 0;
    thread_data_t* thread_data = (thread_data_t*)arg;
    while (thread_data->need) {
        if (thread_data->tid == 3) {
            if (num_out_threads == 3) {
                printf("The End\n");
                break;
            }
            pthread_mutex_lock(&lock);
            store_ind = rand() % 5;
            add_goods(store_ind);
            printf("Store[%d] recieved 500 goods, now it has: %d\n", store_ind, stores[store_ind]);
            pthread_mutex_unlock(&lock);
            usleep(300 * 1000);
        } else {
            pthread_mutex_lock(&lock);
            store_ind = rand() % 5;
            if (stores[store_ind] < thread_data->need) {
                thread_data->need -= stores[store_ind];
                stores[store_ind] = 0;
            } else {
                stores[store_ind] -= thread_data->need;
                thread_data->need = 0;
                num_out_threads++;
            }
            printf("Store[%d] sold products, now it has: %d & Thread[%d] need: %d\n", store_ind, stores[store_ind], thread_data->tid, thread_data->need);
            pthread_mutex_unlock(&lock);
            if (thread_data->need == 0) {
                break;
            }

            usleep(300 * 1000);
        }
    }
    printf("Thread[%d] is out(need: %d, num_out: %d)\n", thread_data->tid, thread_data->need, num_out_threads);
}

void init_threads_data(thread_data_t* threads_data) {
    for (int i = 0; i < NUM_THREADS; i++) {
        threads_data[i].need = rand() % 1001 + 9500;
        // printf("%d\n", threads_data[i].need);
    }
}

void init_stores() {
    for (int i = 0; i < NUM_STORES; i++) {
        stores[i] = rand() % 201 + 900;
        // printf("%d\n", stores[i]);
    }
}

int main() {
    srand(time(NULL));
    int i, rc;
    thread_data_t threads_data[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    init_stores();
    init_threads_data(threads_data);

    for (i = 0; i < NUM_THREADS; i++) {
        threads_data[i].tid = i;
        if ((rc = pthread_create(&threads[i], NULL, run_simulator, (void*)&threads_data[i]))) {
            fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
            return EXIT_FAILURE;
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}