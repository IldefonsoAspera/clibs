#ifndef FIFOR_H
#define FIFOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct fifor_cb
{
    uint8_t  *const   buf;
    volatile uint32_t idx_rd;
    volatile uint32_t idx_wr;
    size_t            buf_size; 
    size_t            bytes_used;
} fifor_t;


#define FIFOR_DEF(_name, _size)                      \
    static uint8_t CONCAT_2(_name, _data)[_size];    \
    static fifor_t _name = {                         \
        .buf = CONCAT_2(_name, _data),               \
        .idx_rd = 0,                                 \
        .idx_wr = 0,                                 \
        .buf_size = _size,                           \
        .bytes_used = 0                              \
    }


/**
 * @brief Checks whether fifo has any item in it or not
 * 
 * @param p_fifo Fifo to check
 * @return true If it is empty
 * @return false If it has any item stored
 */
bool fifor_is_empty(fifor_t *p_fifo);

/**
 * @brief Deletes all items in fifo
 * 
 * @param p_fifo Fifo to clear
 */
void fifor_flush(fifor_t *p_fifo);

/**
 * @brief Inserts item in fifo
 * 
 * @param p_fifo Fifo that will store new item
 * @param length Size in bytes of the item
 * @param p_data Pointer to start of item data
 * @return true If item could be stored in fifo
 * @return false If item could not be stored in fifo (too little available space)
 */
bool fifor_write(fifor_t *p_fifo, uint16_t length, void *p_data);

/**
 * @brief Retrieves head item without deleting it from the fifo
 * 
 * @param p_fifo Fifo from which the item will be retrieved
 * @param p_read_length Size in bytes of the item copied
 * @param p_data Pointer to memory where the item will be copied
 * @return true If item could be retrieved from fifo
 * @return false If queue was empty
 */
bool fifor_peek(fifor_t *p_fifo, uint16_t *p_read_length, void *p_data);

/**
 * @brief Retrieves head item and deletes it from the fifo
 * 
 * @param p_fifo Fifo from which the item will be retrieved
 * @param p_read_length Size in bytes of the item copied
 * @param p_data Pointer to memory where the item will be copied
 * @return true If item could be retrieved from fifo
 * @return false If queue was empty
 */
bool fifor_read(fifor_t *p_fifo, uint16_t *p_read_length, void *p_data);


#ifdef __cplusplus
}
#endif
#endif