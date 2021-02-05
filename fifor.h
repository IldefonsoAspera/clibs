#ifndef FIFOR_H
#define FIFOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


typedef struct fifor_s
{
    uint8_t * const buf;
    volatile uint32_t idx_rd;
    volatile uint32_t idx_wr;
    uint32_t n_items;
    size_t   item_size;
    bool     is_full;
} fifor_t;


#define QUEUE_DEF(_name, _item_type, _n_items)                           \
    static uint8_t CONCAT_2(_name, _array)[sizeof(_item_type)*_n_items]; \
    static fifor_t _name = {                                             \
        .buf = CONCAT_2(_name, _array),                                  \
        .idx_rd = 0,                                                     \
        .idx_wr = 0,                                                     \
        .n_items = _n_items,                                             \
        .item_size = sizeof(_item_type),                                 \
        .is_full = true,                                                 \
    }


bool fifor_get(fifor_t *p_queue, void *p_item);
bool fifor_put(fifor_t *p_queue, void *p_item);
bool fifor_init(fifor_t *p_queue);
void fifor_flush(fifor_t *p_queue);
bool fifor_is_full(fifor_t *p_queue);
bool fifor_is_empty(fifor_t *p_queue);

#ifdef __cplusplus
}
#endif
#endif