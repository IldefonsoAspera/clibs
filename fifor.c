#include "fifor.h"

#include <stdint.h>
#include <stddef.h>

// FIFO capable of containing any (random) size item, but all items must be of the same size
// Buffer does not need to be a power of 2 in size


bool fifor_get(fifor_t *p_queue, void *p_item)
{
    if(!fifor_is_empty(p_queue))
    {
        memcpy(p_item, p_queue->buf[p_queue->idx_rd * p_queue->item_size], p_queue->item_size);
        p_queue->idx_rd = ((p_queue->idx_rd + 1) < p_queue->n_items) ? p_queue->idx_rd++ : 0;
        p_queue->is_full = false;
        return true;
    }
    else
    {
        return false;
    }
}


bool fifor_put(fifor_t *p_queue, void *p_item)
{
    if(!fifor_is_full(p_queue))
    {
        memcpy(p_queue->buf[p_queue->idx_rd * p_queue->item_size], p_item, p_queue->item_size);
        p_queue->idx_wr = ((p_queue->idx_wr + 1) < p_queue->n_items) ? p_queue->idx_wr++ : 0;
        p_queue->is_full = (p_queue->idx_rd == p_queue->idx_wr);
        return true;
    }
    else
    {
        return false;
    }
}


bool fifor_init(fifor_t *p_queue)
{
    fifor_flush(p_queue);  // Not really needed, startup values are ok
}


void fifor_flush(fifor_t *p_queue)
{
    p_queue->idx_rd = 0;
    p_queue->idx_wr = 0;
    p_queue->is_full = false;
}


bool fifor_is_full(fifor_t *p_queue)
{
    return ((p_queue->idx_wr == p_queue->idx_rd) && p_queue->is_full);
}


bool fifor_is_empty(fifor_t *p_queue)
{
    return ((p_queue->idx_wr == p_queue->idx_rd) && !p_queue->is_full);
}