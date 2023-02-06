#include "zemlock.h"
#include <iostream>

using namespace std;

// adult thread
// step 1: enters the room (the capacity of the room goes up by 3)
// as each adult can look after 3 children
// step 2: works (counts to 10000000)
// step 3: leaves the room
void *adult(void *arg) {

    lock_t *lock = (lock_t *) arg;

    // step 1
    lock_post(lock, 3);

    printf("adult entered the room");

    // step 2
    for (int i = 0; i < 10000000; i++);

    printf("adult worked");
    // step 3
    lock_wait(lock, 3);

    printf("adult left the room");
}


// child thread
// step 1: enters the room (the capacity of the room goes down by 1)
// step 2: works (counts to 10000000)
// step 3: leaves the room
void *child(void *arg) {

    lock_t *lock = (lock_t *) arg;


    printf("child trying to enter the room");
    // step 1
    lock_wait(lock);

    printf("child entered the room");
    // step 2
    for (int i = 0; i < 10000000; i++);

    printf("child worked");
    // step 3
    lock_post(lock);

    printf("child left the room");

}