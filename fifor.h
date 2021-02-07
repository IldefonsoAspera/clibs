#ifndef FIFOR_H
#define FIFOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct fifor_s
{
    uint8_t  *const   buf;
    volatile uint32_t idx_rd;
    volatile uint32_t idx_wr;
    size_t            buf_size; 
    size_t            n_in_use;
} fifor_t;


#define FIFOR_DEF(_name, _size)                      \
    static uint8_t CONCAT_2(_name, _array)[_size];   \
    static fifor_t _name = {                         \
        .buf = CONCAT_2(_name, _array),              \
        .idx_rd = 0,                                 \
        .idx_wr = 0,                                 \
        .buf_size = _size,                           \
        .n_in_use = 0                                \
    }


bool fifor_is_empty(fifor_t *p_fifo);
void fifor_flush(fifor_t *p_fifo);
bool fifor_write(fifor_t *p_fifo, uint16_t length, void *p_data);
bool fifor_peek(fifor_t *p_fifo, uint16_t *p_read_length, void *p_data);
bool fifor_read(fifor_t *p_fifo, uint16_t *p_read_length, void *p_data);


#ifdef __cplusplus
}
#endif
#endif