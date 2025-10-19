#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>



void *print_name(void *arg){
    char *name = (char *)arg;
    pthread_t tid = pthread_self();
    printf("I am %s.Thread with ID: %ld is running\n",name,(long unsigned int)tid);
    pthread_exit(NULL);
}

void *print_number(void *arg){
    int *number = (int *)arg;
    pthread_t tid = pthread_self();
    printf("This is number %d.Thread with ID: %ld is running\n", *number,(long unsigned int)tid);
    pthread_exit(NULL);
}


int main(){
    pthread_t thread_name;
    pthread_t thread_number;

    int number = 10;
    char *name = "Quang Vu";
    pthread_create(&thread_name, NULL, print_name, (void*)name);
    pthread_create(&thread_number, NULL, print_number, &number);

    pthread_join(thread_number, NULL);
    pthread_join(thread_name, NULL);
    return 0;
}

