#include <stdio.h>
#include <pthread.h>
 
int a = 0;
 
void* func(void* arg)
{
    for(int i = 0; i < 1000000; i++) {
        a++;
    }
    return 0;
}
 
int main()
{
    pthread_t tid;
    int i, ti;
    int* status = NULL;
    for(i = 0; i < 5; i++) {
        pthread_create(&tid, NULL, func, NULL);
    }
 
    for(ti = 0; ti < 5; ti++) {
        pthread_join(tid, (void**)&status);
    }
    printf("a=%d\n", a);
    return 0;
}