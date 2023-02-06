#include <pthread.h>

typedef struct zemaphore
{
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int value;
} zem_t;

void zem_init(zem_t *, int);
void zem_up(zem_t *, char *);
void zem_down(zem_t *, char *);
void zem_up_n(zem_t *, int, char *);
void zem_down_n(zem_t *, int, char *);