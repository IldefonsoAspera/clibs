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
    bool              is_full;
} fifor_t;

#define FIFOR_DEF(_name, _size)                      \
    static uint8_t CONCAT_2(_name, _array)[_size];   \
    static fifor_t _name = {                         \
        .buf = CONCAT_2(_name, _array),              \
        .idx_rd = 0,                                 \
        .idx_wr = 0,                                 \
        .buf_size = _size,                           \
        .is_full = false,                            \
    }


#ifdef __cplusplus
}
#endif
#endif