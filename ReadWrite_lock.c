#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

long long counter = 0;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void *reader(void *arg) {
    for (int i = 0; i < 5; i++) {
        pthread_rwlock_rdlock(&rwlock);
        printf("[Reader %ld] Counter = %lld\n", (long)pthread_self(), counter);
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
    return NULL;
}

void *writer(void *arg) {
    for (int i = 0; i < 3; i++) {
        pthread_rwlock_wrlock(&rwlock);
        counter++;
        printf("[Writer] Updated counter = %lld\n", counter);
        pthread_rwlock_unlock(&rwlock);
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t w1, w2, r1, r2, r3, r4, r5;
    pthread_create(&w1, NULL, writer, NULL);
    pthread_create(&w2, NULL, writer, NULL);
    pthread_create(&r1, NULL, reader, NULL);
    pthread_create(&r2, NULL, reader, NULL);
    pthread_create(&r3, NULL, reader, NULL);
    pthread_create(&r4, NULL, reader, NULL);
    pthread_create(&r5, NULL, reader, NULL);

    pthread_join(w1, NULL);
    pthread_join(w2, NULL);
    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(r4, NULL);
    pthread_join(r5, NULL);
    return 0;
}

