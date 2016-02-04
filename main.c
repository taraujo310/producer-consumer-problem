#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>

#define nProd 8
#define nCons 3
#define bufferSize 5

//Files.
int buffer[bufferSize];
int count;
int countR;

//Semaphores.
sem_t sProd;
sem_t sCons;
pthread_mutex_t mutualLock;

//Threads of producers and consumers.
pthread_t Producers[nProd];
pthread_t Consumers[nCons];

int stop;
void *writeBuffer(void *id);


void consume(void* id) {
    //consume items in the list.

    int tid = (int)(intptr_t)id;
    printf("O consumidor: %d consumiu a entrada: %d\n",tid, count);
}

void produce(void* id) {
    //Populates the list.
    writeBuffer(id);
}


void* producer(void* producerID) {
    while(stop == 0){
        sem_wait(&sProd);
        pthread_mutex_lock(&mutualLock);
        produce(producerID);
        pthread_mutex_lock(&mutualLock);
        sem_post(&sCons);
    }
    pthread_exit(NULL);
}

void* consumer(void* consumerID) {
    while(stop == 0){
        sem_wait(&sCons);
        pthread_mutex_lock(&mutualLock);
        consume(consumerID);
        pthread_mutex_lock(&mutualLock);
        sem_post(&sProd);
    }
    pthread_exit(NULL);
}



void *writeBuffer(void *id) {
    int tid = (int)(intptr_t)id;
    buffer[count] = tid;
    if((count + 1) < bufferSize){
        count = count + 1;
    } else {
        count = 0;
    }
}





/*----------------------------------------------------*/
int main() {
    stop = 0;
    sem_init(&sProd,0,8);
    sem_init(&sCons,0,0);
    count = 0;
    countR = 0;
    int tp;
    int tc;
    int i;
    for(i = 0; i < nProd; i++){
        tp = pthread_create(&Producers[i], NULL, producer, (void*)(intptr_t)i);
    }
    for(i = 0; i < nCons; i++){
        int id = -i;
        tc = pthread_create(&Consumers[id], NULL, consumer, (void*)(intptr_t)id);
    }

    pthread_exit(NULL);
}
