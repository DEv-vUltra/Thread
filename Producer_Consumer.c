#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>


#define COUNT 10

int data = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int data_ready = 0;


void *producer(void *arg){
    printf("Transferring data.......\n");
    for(int i = 0; i < COUNT; i++){
        pthread_mutex_lock(&lock); 
        data = rand() % 100;
        data_ready = 1;
        
        sleep(1);
        printf("Produce data: %d\n", data); 

        pthread_cond_signal(&cond);
        // while(data_ready){
        //     pthread_cond_wait(&cond, &wait)
        // } 
        // deadlock phenomenon      
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    
    

    return NULL;
}

void *consumer(void *arg){
    for(int i = 0; i < COUNT; i++){
        pthread_mutex_lock(&lock);
        while(!data_ready){
            printf("Consumer. Waiting for transferring data...\n");
            pthread_cond_wait(&cond, &lock);
        }

        pthread_mutex_unlock(&lock);
        printf("Consumed data: %d\n", data);
        data_ready = 0;

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}


int main(){
    srand(time(NULL));
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, producer, NULL);
    pthread_create(&thread2, NULL, consumer, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}