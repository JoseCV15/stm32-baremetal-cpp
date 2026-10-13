#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "status.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
    uint8_t *data;
    uint32_t capacity;
    volatile uint32_t head; // next write position
    volatile uint32_t tail; // next read position
} rb_t;

typedef enum {
    RB_OK = 0,
    RB_ERR_NULL,
    RB_ERR_CAPACITY,
    RB_ERR_ARGUMENT
} rb_status_t;

rb_status_t  rb_init(rb_t *rb, uint8_t *storage, uint32_t capacity);
rb_status_t rb_reset(rb_t *rb);
bool rb_is_empty(const rb_t *rb);
bool rb_is_full(const rb_t *rb);
uint32_t rb_free(const rb_t *rb);
uint32_t rb_size(const rb_t *rb);

/*
* Write up to len bytes. Returns RB_OK on success.
* written receives the number of bytes acutally written
*/
rb_status_t rb_write(rb_t *rb, const uint8_t *src, uint32_t len, uint32_t *written);

/*
* Read up to len bytes. Returns RB_OK on success.
* read_count receives the number actually read
*/
rb_status_t rb_read(rb_t *rb, uint8_t *dst, uint32_t len, uint32_t *read_count);

rb_status_t rb_peek(const rb_t *rb, uint8_t *dst, uint32_t len, uint32_t *copied);
rb_status_t rb_discard(rb_t *rb, uint32_t len, uint32_t *discarded);





#ifdef __cplusplus
}
#endif

#endif // RINGBUFFER_H