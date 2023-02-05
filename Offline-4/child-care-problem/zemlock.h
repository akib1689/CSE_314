/**
 * header file for zemlock.c
 * contains struct lock 
 * has zem_t variable
 * has mutex for protecting the zem_t variable
*/

#ifndef ZEMLOCK_H
#define ZEMLOCK_H 

#include "../zemaphore/zemaphore.h"

typedef struct {
    zem_t zem;
    pthread_mutex_t mutex;
} lock_t;

void lock_init(lock_t *);
void lock_post(lock_t *);
void lock_wait(lock_t *);
void lock_post(lock_t *, int);
void lock_wait(lock_t *, int);

#endif