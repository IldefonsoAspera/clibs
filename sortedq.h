#ifndef SORTEDQ_H
#define SORTEDQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


typedef struct sortedq_s
{
    uint8_t * const buf;
    volatile uint32_t idx_rd;
    volatile uint32_t idx_wr;
    volatile uint32_t n_items;
    uint32_t max_items;
    size_t   item_size;
} sortedq_t;


#define SORTEDQ_DEF(_name, _item_type, _max_items)                         \
    static uint8_t CONCAT_2(_name, _data)[sizeof(_item_type)*_max_items];  \
    static sortedq_t _name = {                                             \
        .buf = CONCAT_2(_name, _data),                                     \
        .idx_rd = 0,                                                       \
        .idx_wr = 0,                                                       \
        .max_items = _max_items,                                           \
        .item_size = sizeof(_item_type),                                   \
        .n_items = 0                                                       \
    }












#ifdef __cplusplus
}
#endif
#endif