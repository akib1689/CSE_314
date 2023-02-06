#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>


pthread_cond_t carer_entered;
pthread_cond_t duty_free;
pthread_mutex_t lock;


int child_play_loop = 100000;
int max_child_per_carer, num_child;
int room_resource;
int children_in_room;
int remaining_children;

void *carer_watch(void* data){
    pthread_mutex_lock(&lock);
        pthread_cond_broadcast(&carer_entered); 
        printf("Carer entered the room\n");
        room_resource += max_child_per_carer;
        //wait for duty_free
        while( remaining_children > 0 ) {
            pthread_cond_wait(&duty_free, &lock);
        }
        //carer leaves the room
        room_resource -= max_child_per_carer;
    pthread_mutex_unlock(&lock);
}

void *child_play(void* data) {
    int child_id = *((int*) data);
    int i, x = 0;

    pthread_mutex_lock(&lock);
        while( room_resource == 0 || children_in_room == max_child_per_carer) {
            pthread_cond_wait(&carer_entered, &lock);
        }
        printf("Child %d has entered the room\n", child_id);
        children_in_room++;
    pthread_mutex_unlock(&lock);


    // child playing
    for(i = 0; i < child_play_loop; i++){
        x+=1;
    }

    pthread_mutex_lock(&lock);
        printf("Child %d has finished playing and leaving room\n", child_id);
        remaining_children--;
        // leave room
        children_in_room--;
        // signal duty free if #remaining_children is 0
        if( remaining_children == 0 ){
            pthread_cond_signal(&duty_free);
        }
    pthread_mutex_unlock(&lock);
}

int main(int argc, char *argv[])
{
    int *child_thread_id;
    pthread_t *child_thread, carer_thread;
    int i;
    if (argc < 3) {
        printf("./child-care #max_child_per_carer #num_child e.g. ./exe 3 10 \n");
        exit(1);
    } else {
        max_child_per_carer = atoi(argv[1]);
        num_child = atoi(argv[2]);
    }

    room_resource = 0;
    children_in_room = 0;
    remaining_children = num_child;

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\nmutex init has failed\n");
        exit(1);
    }

    child_thread_id = (int *)malloc(sizeof(int) * num_child);
    child_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_child);
    for( i = 0; i < num_child; i++) {
        child_thread_id[i] = i;
        pthread_create(&child_thread[i], NULL, child_play, (void *)&child_thread_id[i]);
    }    
    pthread_create(&carer_thread,  NULL, carer_watch, NULL);


    for( i = 0; i < num_child; i++) {
        pthread_join(child_thread[i], NULL);
        printf("Playtime is over for child %d\n", i);
    }
    pthread_join(carer_thread, NULL);
    printf("Carer has finished duty\n");

    free(child_thread);
    free(child_thread_id);
    pthread_mutex_destroy(&lock);
}