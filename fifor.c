#include "fifor.h"

#include <stddef.h>

// FIFO that can contain arbitrary (random) sized elements, each one with a different size
// It uses a 16b sized variable for the length, so max pkt length is 65KB

/*
data to store in the queue
 ---------
| payload |
 ---------
  |
  | payload gets its size appended at the beginning
  V
 ----------------
| size | payload |
 ---------------- 
  |
  |
  | packet is inserted in queue, along with others
  V
 ------------------------------------------------
| size1 | payload1 | size2 | payload2 | ...      |
 ------------------------------------------------
*/

// TODO falta actualizar is_full variable
// TODO revisar si se puede meter un ultimo dato en wr==rd porque igual borra el dato mas antiguo

static size_t fifor_free_space(fifor_t *p_fifo)
{
    if(p_fifo->is_full)
    {
        return p_fifo->buf_size;
    }
    else
    {
        return ((p_fifo->idx_wr - p_fifo->idx_rd) % p_fifo->buf_size);
    }
}


static size_t fifor_filled_space(fifor_t *p_fifo)
{
    return ((p_fifo->idx_wr - p_fifo->idx_rd) % p_fifo->buf_size);
}

bool fifor_is_empty(fifor_t *p_fifo)
{
    return ((p_fifo->idx_rd == p_fifo->idx_wr) && !p_fifo->is_full);
}



static uint8_t fifor_get(fifor_t *p_fifo)
{
    uint8_t out_val = p_fifo->buf[p_fifo->idx_rd];

    p_fifo->idx_rd = (p_fifo->idx_rd + 1) % p_fifo->buf_size;
    return out_val;
}


static void fifor_put(fifor_t *p_fifo, uint8_t data)
{
    p_fifo->buf[p_fifo->idx_wr] = data;
    p_fifo->idx_wr = (p_fifo->idx_wr + 1) % p_fifo->buf_size;
}


bool fifor_write(fifor_t *p_fifo, uint16_t length, void *p_data)
{
    uint8_t *p_orig = p_data;

    if(length > 0 && fifor_free_space > (length + sizeof(length)))
    {
        fifor_put(p_fifo, length>>8);
        fifor_put(p_fifo, length&0xFF);
        while(length-- > 0)
        {
            fifor_put(p_fifo, *p_orig++);
        }
        return true;
    }
    else
    {
        return false;
    }
}




bool fifor_peek(fifor_t *p_fifo, uint16_t *p_length, void *p_data)
{
    uint16_t length;
    uint8_t *p_dest = p_data;
    uint32_t idx_orig = p_fifo->idx_rd;

    if(!fifor_is_empty(p_fifo))
    {
        length = (fifor_get(p_fifo)<<8) + fifor_get(p_fifo);
        *p_length = length;
        while (length-- > 0)
        {
            *p_dest++ = p_fifo->buf[idx_orig];
            idx_orig = (idx_orig + 1) % p_fifo->buf_size;
        }
        return true;   
    }
    else
    {
        return false;
    }
}


bool fifor_read(fifor_t *p_fifo, size_t *p_length, void *p_data)
{
    uint16_t length;

    if(fifor_peek(p_fifo, &length, p_data))
    {
        p_fifo->idx_rd = (p_fifo->idx_rd + length) % p_fifo->buf_size;
        return true;
    }
    else
    {
        return false;
    }
}