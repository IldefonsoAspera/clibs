#include "fifor.h"

#include <stddef.h>

// FIFO that can contain arbitrary (random) sized elements, each one with a different size
// It uses a 16b sized variable for the length, so max pkt length is (2^16 - 1) bytes

// Only safe as long as there is 1 reader and 1 writer, and no flushes are performed
// while reading or writing
// p_fifo->bytes_used variable must be updated atomically (all its bytes on the same instruction)

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
 ----------------------------------------------------
| ... || size1 | payload1 || size2 | payload2 || ... |
 ----------------------------------------------------
*/


#define FIFOR_INCR_IDX(_idx)    ((_idx) = ((_idx) + 1) % p_fifo->buf_size)



static size_t fifor_free_space(fifor_t *p_fifo)
{
    return p_fifo->buf_size - p_fifo->bytes_used;
}


static size_t fifor_filled_space(fifor_t *p_fifo)
{
    return p_fifo->bytes_used;
}


bool fifor_is_empty(fifor_t *p_fifo)
{
    return p_fifo->bytes_used == 0;
}


void fifor_flush(fifor_t *p_fifo)
{
    p_fifo->idx_rd = 0;
    p_fifo->idx_wr = 0;
    p_fifo->bytes_used = 0;
}


bool fifor_write(fifor_t *p_fifo, uint16_t length, void *p_data)
{
    uint8_t  *p_orig = p_data;
    size_t   req_length = length;

    if(fifor_free_space(p_fifo) >= (length + sizeof(length)))
    {
        p_fifo->buf[p_fifo->idx_wr] = length>>8;
        FIFOR_INCR_IDX(p_fifo->idx_wr);
        p_fifo->buf[p_fifo->idx_wr] = length;
        FIFOR_INCR_IDX(p_fifo->idx_wr);

        while(length-- > 0)
        {
            p_fifo->buf[p_fifo->idx_wr] = *p_orig++;
            FIFOR_INCR_IDX(p_fifo->idx_wr);
        }

        p_fifo->bytes_used += req_length + sizeof(length);    // sizeof(length) is not an errata
        return true;
    }
    else
    {
        return false;
    }
}


bool fifor_peek(fifor_t *p_fifo, uint16_t *p_read_length, void *p_data)
{
    uint16_t length;
    uint8_t  *p_dest = p_data;
    uint32_t idx_orig = p_fifo->idx_rd;

    if(!fifor_is_empty(p_fifo))
    {
        length = p_fifo->buf[idx_orig]<<8;
        FIFOR_INCR_IDX(idx_orig);
        length += p_fifo->buf[idx_orig];        
        FIFOR_INCR_IDX(idx_orig);
        *p_read_length = length;

        while (length-- > 0)
        {
            *p_dest++ = p_fifo->buf[idx_orig];
            FIFOR_INCR_IDX(idx_orig);
        }
        return true;   
    }
    else
    {
        return false;
    }
}


bool fifor_read(fifor_t *p_fifo, uint16_t *p_read_length, void *p_data)
{
    uint16_t length;

    if(fifor_peek(p_fifo, &length, p_data))
    {
        *p_read_length = length;
        p_fifo->idx_rd = (p_fifo->idx_rd + length + sizeof(length)) % p_fifo->buf_size;
        p_fifo->bytes_used -= length + sizeof(length);
        return true;
    }
    else
    {
        return false;
    }
}