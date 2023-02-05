/**
 * This implementation of the reader-writer lock is a writer-preference lock.
 * if there are any writers waiting, then readers will have to wait.
*/


#include "rwlock.h"

void InitalizeReadWriteLock(read_write_lock *rw) {
  pthread_mutex_init(&rw->lock, NULL);
  pthread_cond_init(&rw->read, NULL);
  pthread_cond_init(&rw->write, NULL);
  rw->writers = 0;
  rw->readers = 0;
  rw->write_waiters = 0;
}

void ReaderLock(read_write_lock *rw) {
  // lock the mutex so that no other thread can access the this structure
  pthread_mutex_lock(&rw->lock);
  while (rw->writers > 0 || rw->write_waiters > 0) {
    // if there are any writers or writers waiting, then wait
    // at the time of sleeping, the mutex is unlocked by the OS
    // after waking up, the mutex is locked again
    pthread_cond_wait(&rw->read, &rw->lock);
  }
  // there is no writer or writer waiting, 
  // so we can increment the reader count
  rw->readers++;

  // unlock the mutex so that other threads can access the structure
  pthread_mutex_unlock(&rw->lock);
}

void WriterLock(read_write_lock *rw) {
  // lock the mutex so that no other thread can access the this structure
  pthread_mutex_lock(&rw->lock);
  rw->write_waiters++;
  while (rw->readers > 0 || rw->writers > 0) {
    // if there are any readers or writers, then wait
    // at the time of sleeping, the mutex is unlocked by the OS
    // after waking up, the mutex is locked again
    pthread_cond_wait(&rw->write, &rw->lock);
  }
  // there is no reader or writer,
  // so we can increment the writer count and 
  // decrement the writer waiting count
  rw->write_waiters--;
  rw->writers++;
  pthread_mutex_unlock(&rw->lock);
}

void ReaderUnlock(read_write_lock *rw) {
  pthread_mutex_lock(&rw->lock);
  rw->readers--;
  if (rw->readers == 0 && rw->write_waiters > 0) {
    pthread_cond_signal(&rw->write);
  }
  pthread_mutex_unlock(&rw->lock);
}

void WriterUnlock(read_write_lock *rw) {
  pthread_mutex_lock(&rw->lock);
  rw->writers--;
  if (rw->write_waiters > 0) {
    pthread_cond_signal(&rw->write);
  } else {
    pthread_cond_broadcast(&rw->read);
  }
  pthread_mutex_unlock(&rw->lock);
}
