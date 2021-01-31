#ifndef FIFO_H
#define FIFO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct fifo_t Fifo_t;

void fifo_init(Fifo_t *p_fifo, uint8_t *p_buffer, size_t length, bool is_atomic);
bool fifo_put(Fifo_t *p_fifo, uint8_t value);
bool fifo_get(Fifo_t *p_fifo, uint8_t *p_value);
void fifo_flush(Fifo_t *p_fifo);

size_t fifo_write(Fifo_t *p_fifo, size_t data_length, uint8_t *p_data);
size_t fifo_read(Fifo_t *p_fifo, size_t data_length, uint8_t *p_data);


#ifdef __cplusplus
}
#endif
#endif