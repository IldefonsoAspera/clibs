#include "fifoc.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>

// FIFO capable of containing any (configurable) size item, but all items must be of the same size
// Buffer does not need to be a power of 2 in size
// WARNING this fifo is only safe as long as there is 1 reader and 1 writer only

#define FIFOC_INCR_IDX(_idx)    ((_idx) = ((_idx) + 1) % p_fifo->max_items)



bool fifoc_is_full(fifoc_t *p_fifo)
{
    return p_fifo->n_items == p_fifo->max_items;
}


bool fifoc_is_empty(fifoc_t *p_fifo)
{
    return p_fifo->n_items == 0;
}


bool fifoc_get(fifoc_t *p_fifo, void *p_item)
{
    if(!fifoc_is_empty(p_fifo))
    {
        uint8_t *p_item_fifo = &p_fifo->buf[p_fifo->idx_rd * p_fifo->item_size];

        memcpy(p_item, p_item_fifo, p_fifo->item_size);
        FIFOC_INCR_IDX(p_fifo->idx_rd);
        p_fifo->n_items--;
        return true;
    }
    else
    {
        return false;
    }
}


bool fifoc_put(fifoc_t *p_fifo, void *p_item)
{
    if(!fifoc_is_full(p_fifo))
    {
        uint8_t *p_item_fifo = &p_fifo->buf[p_fifo->idx_wr * p_fifo->item_size];

        memcpy(p_item_fifo, p_item, p_fifo->item_size);
        FIFOC_INCR_IDX(p_fifo->idx_wr);
        p_fifo->n_items++;
        return true;
    }
    else
    {
        return false;
    }
}


bool fifoc_init(fifoc_t *p_fifo)
{
    fifoc_flush(p_fifo);  // Not really needed, startup values are ok
}


void fifoc_flush(fifoc_t *p_fifo)
{
    p_fifo->idx_rd = 0;
    p_fifo->idx_wr = 0;
    p_fifo->n_items = 0;
}