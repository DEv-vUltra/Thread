#include <stdio.h>

#include <pthread.h>
#include <unistd.h>



long long counter = 0; // shared variable
pthread_mutex_t lock;

void *thread(void *arg){
    for(int i = 0; i < 1000000; i++){
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock); 
    }
    return NULL;
}


int main(){
    pthread_t thread_1;
    pthread_t thread_2;
    pthread_t thread_3;

    pthread_create(&thread_1, NULL, thread, NULL);
    pthread_create(&thread_2, NULL, thread, NULL);
    pthread_create(&thread_3, NULL, thread, NULL);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    pthread_join(thread_3, NULL);


    printf("Counter = %lld\n",counter);
    return 0;
}
