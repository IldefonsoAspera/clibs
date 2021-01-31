
#include "fifo.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "utils.h"


struct fifo_t 
{
    uint8_t  *buffer;
    uint32_t next_rd;
    uint32_t next_wr;
    size_t   mask;
    bool     is_atomic;
};


inline size_t fifo_filled_space(Fifo_t *p_fifo)
{
    return p_fifo->next_wr - p_fifo->next_rd;
}


inline size_t fifo_free_space(Fifo_t *p_fifo)
{
    return (p_fifo->mask + 1) - fifo_filled_space(p_fifo);
}


// Returns number of bytes from wr_pos to end of lineal array (before wrap-around)
inline size_t fifo_lineal_space(Fifo_t *p_fifo)
{
    return (p_fifo->mask + 1) - (p_fifo->next_wr & (p_fifo->mask + 1));
}


inline void fifo_insert(Fifo_t *p_fifo, uint8_t value)
{
    p_fifo->buffer[p_fifo->next_wr & p_fifo->mask] = value;
    p_fifo->next_wr++;
}


inline uint8_t fifo_retrieve(Fifo_t *p_fifo)
{
    uint8_t output = p_fifo->buffer[p_fifo->next_rd & p_fifo->mask];
    p_fifo->next_rd++;
    return output;
}



void fifo_init(Fifo_t *p_fifo, uint8_t *p_buffer, size_t length, bool is_atomic)
{
    // TODO
    util_assert(p_fifo    != NULL);
    util_assert(p_buffer  != NULL);
    util_assert(length    != 0);
    util_assert((length & (length - 1)) == 0);	// Assert length is power of two

    p_fifo->buffer    = p_buffer;
    p_fifo->next_rd   = 0;
    p_fifo->next_wr   = 0;
    p_fifo->mask      = length - 1;
    p_fifo->is_atomic = is_atomic;
}


bool fifo_put(Fifo_t *p_fifo, uint8_t value)
{
    if(fifo_free_space(p_fifo) != 0)
    {
        fifo_insert(p_fifo, value);
        return true;
    }
    return false;
}


bool fifo_get(Fifo_t *p_fifo, uint8_t *p_value)
{
    if(fifo_filled_space(p_fifo) != 0)
    {
        *p_value = fifo_retrieve(p_fifo);
        return true;
    }
    return false;
}


void fifo_flush(Fifo_t *p_fifo)
{
    p_fifo->next_rd = 0;
    p_fifo->next_wr = 0;
}


size_t fifo_write(Fifo_t *p_fifo, size_t data_length, uint8_t *p_data)
{
/*	size_t  free_lineal = min(fifo_get_free_space(p_fifo), fifo_get_lineal_space(p_fifo));
    size_t  n_copied    = min(data_length, free_lineal);
    uint8_t *p_write    = &p_fifo->buffer[p_fifo->next_wr & p_fifo->mask];

    memcpy(p_write, p_data, n_copied);
    p_fifo->next_wr += n_copied;

    if(data_length > n_copied)
    {
        size_t n_copied_wrap = min(data_length - n_copied, fifo_get_free_space(p_fifo));
        // Always has to start copying to buffer[0] because fifo just wrapped-around
        memcpy(p_fifo->buffer, p_data + n_copied, n_copied_wrap);
        p_fifo->next_wr += n_copied_wrap;
        n_copied += n_copied_wrap;
    }
    return n_copied;
*/

    size_t wr_size = min(fifo_free_space(p_fifo), data_length);
    int i;

    for(i=0; i<wr_size; i++)
        fifo_insert(p_fifo, p_data[i]);

    return wr_size;
}


size_t fifo_read(Fifo_t *p_fifo, size_t data_length, uint8_t *p_data)
{
    size_t rd_size = min(fifo_filled_space(p_fifo), data_length);
    int i;
    
    for(i=0; i<rd_size; i++)
        p_data[i] = fifo_retrieve(p_fifo);

    return rd_size;
}