#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CHILDREN 3
#define NUM_ADULTS 2
#define NUM_CHILDREN 6

sem_t adult_sem;
sem_t child_sem;
sem_t mutex;
int children = 0;

void *adult_thread(void *arg) {
    int id = *(int *)arg;

    while (1) {
        sem_wait(&adult_sem);
        sem_wait(&mutex);

        children += 3;
        printf("Adult %d entered, children now = %d\n", id, children);

        sem_post(&child_sem);
        sem_post(&child_sem);
        sem_post(&child_sem);

        sem_post(&mutex);
    }

    return NULL;
}

void *child_thread(void *arg) {
    int id = *(int *)arg;

    while (1) {
        sem_wait(&child_sem);
        sem_wait(&mutex);

        children--;
        printf("Child %d left, children now = %d\n", id, children);

        sem_post(&adult_sem);

        sem_post(&mutex);
    }

    return NULL;
}

int main(void) {
    pthread_t adult_tid[NUM_ADULTS];
    pthread_t child_tid[NUM_CHILDREN];

    sem_init(&adult_sem, 0, NUM_ADULTS);
    sem_init(&child_sem, 0, 0);
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < NUM_ADULTS; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&adult_tid[i], NULL, adult_thread, id);
    }

    for (int i = 0; i < NUM_CHILDREN; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&child_tid[i], NULL, child_thread, id);
    }

    for (int i = 0; i < NUM_ADULTS; i++) {
        pthread_join(adult_tid[i], NULL);
    }

    for (int i = 0; i < NUM_CHILDREN; i++) {
        pthread_join(child_tid[i], NULL);
    }

    sem_destroy(&adult_sem);
    sem_destroy(&child_sem);
    sem_destroy(&mutex);

    return 0;
}
