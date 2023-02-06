#include "zemaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

using namespace std;

zem_t *lock;
int num_parent, num_child;

// adult thread
// step 1: enters the room (the capacity of the room goes up by 3)
// as each adult can look after 3 children
// step 2: works (counts to 10000000)
// step 3: leaves the room
void *adult(void *arg)
{

    int *adult_id = (int *)arg;
    printf("adult %d entered the room\n", (*adult_id));

    // step 1
    zem_up_n(lock, 3);

    // step 2
    for (int i = 0; i < 10000000; i++)
        ;

    printf("adult %d worked\n", (*adult_id));
    // step 3
    zem_down_n(lock, 3);

    printf("adult %d left the room\n", (*adult_id));

    // return NULL;
}

// child thread
// step 1: enters the room (the capacity of the room goes down by 1)
// step 2: works (counts to 10000000)
// step 3: leaves the room
void *child(void *arg)
{
    // printf("child\n");

    int *child_id = (int *)arg;
    // lock_t *lock = (lock_t *)arg;

    printf("child %d trying to enter the room\n", (*child_id));
    // step 1
    zem_down(lock);

    printf("child %d entered the room\n", (*child_id));
    // step 2
    for (int i = 0; i < 10000000; i++)
        ;

    printf("child %d worked\n", (*child_id));
    // step 3
    zem_up(lock);

    printf("child %d left the room\n", (*child_id));
}

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        printf("2 command line argument required\n");
        exit(1);
    }
    // printf("here\n");
    // return 0;

    num_parent = atoi(argv[1]);
    num_child = atoi(argv[2]);
    // printf("%d , %d\n", num_parent, num_child);

    int *child_thread_id = (int *)malloc(sizeof(int) * num_child);
    int *parent_thread_id = (int *)malloc(sizeof(int) * num_parent);
    pthread_t *child_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_child);
    pthread_t *parent_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_parent);
    lock = (zem_t *)malloc(sizeof(zem_t) * 1);
    zem_init(lock, 0);
    for (int i = 0; i < num_child; i++)
    {
        // printf("here too\n");
        child_thread_id[i] = i;
        pthread_create(&child_thread[i], NULL, child, (void *)&child_thread_id[i]);
    }

    for (int i = 0; i < num_parent; i++)
    {
        parent_thread_id[i] = i;
        pthread_create(&parent_thread[i], NULL, adult, (void *)&parent_thread_id[i]);
    }

    for (int i = 0; i < num_child; i++)
    {
        pthread_join(child_thread[i], NULL);
        printf("child %d has finished\n", child_thread_id[i]);
    }

    for (int i = 0; i < num_parent; i++)
    {
        pthread_join(parent_thread[i], NULL);
        printf("adult %d has finished\n", parent_thread_id[i]);
    }

    free(child_thread);
    free(child_thread_id);

    free(parent_thread);
    free(parent_thread_id);

    return 0;
}