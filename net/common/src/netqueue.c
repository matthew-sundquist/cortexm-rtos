
#include <stddef.h>
#include "netqueue.h"

// arbitrarily chosen, should be the max amount of netbufs allocated
#define SEM_MAX_COUNT 1000

#define container_of(ptr, type, member) \ 
    ((type *)((char *)(ptr) - offsetof(type, member)))

bool netqueue_init(netqueue_t *nq)
{
    if (!nq)
    {
        return false;
    }

    ts_queue_init(&nq->queue);
    sem_init(&nq->sem, SEM_MAX_COUNT, 0);

    return true;
}

bool netqueue_push(netqueue_t *nq, netbuf_t *nb)
{
    if (!nq || !nb)
    {
        return false;
    }

    if (!ts_queue_push(&nq->queue, &nb->link))
    {
        return false;
    }

    sem_release(&nq->sem);
    return true;
}

bool netqueue_pop(netqueue_t *nq, netbuf_t *nb)
{
    if (!nq || !nb)
    {
        return false;
    }

    sem_aquire(&nq->sem);

    queue_link_t *link;
    if (!ts_queue_pop(&nq->queue, link))
    {
        return false;
    }

    nb = container_of(link, netbuf_t, link);

    return true;
}


