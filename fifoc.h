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
    static uint8_t CONCAT_2(_name, _data)[sizeof(_item_type)*_max_items]; \
    static fifoc_t _name = {                                               \
        .buf = CONCAT_2(_name, _data),                                    \
        .idx_rd = 0,                                                       \
        .idx_wr = 0,                                                       \
        .max_items = _max_items,                                           \
        .item_size = sizeof(_item_type),                                   \
        .n_items = 0                                                       \
    }


/**
 * @brief Retrieves item and deletes it from the fifo
 * 
 * @param p_fifo Fifo from which the item will be retrieved
 * @param p_item Pointer to memory where the item will be copied
 * @return true If item could be retrieved from fifo
 * @return false If queue was empty
 */
bool fifoc_get(fifoc_t *p_fifo, void *p_item);

/**
 * @brief Inserts item in fifo
 * 
 * @param p_fifo Fifo that will store new item
 * @param length Size in bytes of the item
 * @param p_data Pointer to start of item data
 * @return true If item could be stored in fifo
 * @return false If item could not be stored in fifo (too little available space)
 */
bool fifoc_put(fifoc_t *p_fifo, void *p_item);

/**
 * @brief Initializes (flushes) fifo. Not really needed.
 * 
 * @param p_fifo 
 * @return true 
 * @return false 
 */
bool fifoc_init(fifoc_t *p_fifo);

/**
 * @brief Deletes all items in fifo
 * 
 * @param p_fifo Fifo to empty
 */
void fifoc_flush(fifoc_t *p_fifo);

/**
 * @brief Checks whether fifo has space for more items
 * 
 * @param p_fifo Fifo to check
 * @return true If there is some available slots
 * @return false If fifo is full
 */
bool fifoc_is_full(fifoc_t *p_fifo);

/**
 * @brief Checks whether fifo has any item in it or not
 * 
 * @param p_fifo Fifo to check
 * @return true If it is empty
 * @return false If it has any item stored
 */
bool fifoc_is_empty(fifoc_t *p_fifo);

#ifdef __cplusplus
}
#endif
#endif