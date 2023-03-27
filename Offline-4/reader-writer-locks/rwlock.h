/**
 * This is the header file for the reader-writer lock.
 * it has the following functions:
 * 1. rwlock_init                   // initialize the lock
 * 2. rwlock_acquire_readlock       // acquire the read lock
 * 3. rwlock_release_readlock       // release the read lock
 * 4. rwlock_acquire_writelock      // acquire the write lock
 * 5. rwlock_release_writelock      // release the write lock
 *
 *
 * two implementations of the lock are provided:
 * 1. reader-preference lock        // readers have priority
 * 2. writer-preference lock        // writers have priority
 *
 * @author:  Akib - 1805086
 */

#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

struct read_write_lock {
  pthread_mutex_t lock;
  pthread_cond_t read;
  pthread_cond_t write;
  int writers;
  int readers;
  int write_waiters;
};

void InitalizeReadWriteLock(struct read_write_lock* rw);
void ReaderLock(struct read_write_lock* rw);
void ReaderUnlock(struct read_write_lock* rw);
void WriterLock(struct read_write_lock* rw);
void WriterUnlock(struct read_write_lock* rw);

#endif
