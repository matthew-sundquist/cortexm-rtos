
#include "ts_queue.h"
#include "mutex.h"

bool ts_queue_init(ts_queue_t *q)
{
    if (!q)
    {
        return false;
    }

    head = NULL;
    tail = NULL;
    mutex_init(&mut);
    
    return true;
}

// push to tail
bool ts_queue_push(ts_queue_t *q, queue_link_t *ql)
{
    if (!q || !data)
    {
        return false;
    }

    mutex_aquire(&q->mut);

    if (!q->head)
    {
        q->head = ql;
        q->tail = ql;
    }
    else
    {
        q->tail->next = ql;
        q->tail = ql;
    }

    ql->next = NULL;
    mutex_release(&q->mut);
    return true;
}

// pop from head
bool ts_queue_pop(ts_queue_t *q, queue_link_t *ql)
{
    if (!q || !ql)
    {
        return false;
    }

    mutex_aquire(&q->mut);

    if (!q->head)
    {
        mutex_release(&q->mut);
        return false;
    }

    ql = q->head;

    if (q->head == q->tail)
    {
        q->tail = NULL;
    }
    
    q->head = q->head->next;

    ql->next = NULL;

    return true;
}
