#include "zemlock.h"

void lock_init(lock_t *lock)
{
    zem_init(&lock->zem, 0);
    pthread_mutex_init(&lock->mutex, NULL);
}
void lock_post(lock_t *lock)
{
    pthread_mutex_lock(&lock->mutex);
    zem_up(&lock->zem);
    pthread_mutex_unlock(&lock->mutex);
}
void lock_wait(lock_t *lock)
{
    pthread_mutex_lock(&lock->mutex);
    zem_down(&lock->zem);
    pthread_mutex_unlock(&lock->mutex);
}

void lock_post(lock_t *lock, int n)
{
    pthread_mutex_lock(&lock->mutex);
    for (int i = 0; i < n; i++)
    {
        zem_up(&lock->zem);
    }
    pthread_mutex_unlock(&lock->mutex);
}

void lock_wait(lock_t *lock, int n)
{
    pthread_mutex_lock(&lock->mutex);
    for (int i = 0; i < n; i++)
    {
        zem_down(&lock->zem);
    }
    pthread_mutex_unlock(&lock->mutex);
}