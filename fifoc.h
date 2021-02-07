#ifndef FIFOC_H
#define FIFOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


typedef struct fifoc_s
{
    uint8_t * const buf;
    volatile uint32_t idx_rd;
    volatile uint32_t idx_wr;
    volatile uint32_t n_items;
    uint32_t max_items;
    size_t   item_size;
} fifoc_t;


#define FIFOC_DEF(_name, _item_type, _max_items)                           \
    static uint8_t CONCAT_2(_name, _array)[sizeof(_item_type)*_max_items]; \
    static fifoc_t _name = {                                               \
        .buf = CONCAT_2(_name, _array),                                    \
        .idx_rd = 0,                                                       \
        .idx_wr = 0,                                                       \
        .max_items = _max_items,                                           \
        .item_size = sizeof(_item_type),                                   \
        .n_items = 0                                                       \
    }


bool fifoc_get(fifoc_t *p_queue, void *p_item);
bool fifoc_put(fifoc_t *p_queue, void *p_item);
bool fifoc_init(fifoc_t *p_queue);
void fifoc_flush(fifoc_t *p_queue);
bool fifoc_is_full(fifoc_t *p_queue);
bool fifoc_is_empty(fifoc_t *p_queue);

#ifdef __cplusplus
}
#endif
#endif