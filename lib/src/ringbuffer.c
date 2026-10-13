#include "ringbuffer.h"

rb_status_t  rb_init(rb_t *rb, uint8_t *storage, uint32_t capacity)
{
    if (!rb || !storage)
        return RB_ERR_NULL;
    if (capacity == 0U) 
        return RB_ERR_CAPACITY;

    rb->data = storage;
    rb->capacity = capacity;
    rb->head = 0U;
    rb->tail = 0U;
    return RB_OK;
}

rb_status_t rb_reset(rb_t *rb)
{
    if (!rb || !rb->data) 
        return RB_ERR_NULL;

    rb->head = 0U;
    rb->tail = 0U;
    return RB_OK;
}

bool rb_is_empty(const rb_t *rb)
{
    if (!rb || !rb->data || rb->capacity == 0U) {
        return true;
    }
    return rb->head == rb->tail;
}

bool rb_is_full(const rb_t *rb)
{
    if (!rb || !rb->data || rb->capacity == 0U) {
        return false;
    }
    return ((rb->head + 1U) % rb->capacity) == rb->tail;
}

uint32_t rb_free(const rb_t *rb)
{
    if (!rb || !rb->data || rb->capacity == 0U) {
        return 0U;
    }
    return (rb->capacity -1U) - rb_size(rb);
}

uint32_t rb_size(const rb_t *rb)
{
    uint32_t head, tail;
    if (!rb || !rb->data || rb->capacity == 0U) {
        return 0U;
    }
    head = rb->head;
    tail = rb->tail;

    if (head >= tail) {
        return head - tail;
    }
    return rb->capacity - tail + head;
}

rb_status_t rb_write(rb_t *rb, const uint8_t *src, uint32_t len, uint32_t *written)
{
    uint32_t count = 0U;
    uint32_t next;
    if (!written) {
        return RB_ERR_NULL;
    }
    *written = 0U;
    if (!rb || !rb->data) {
        return RB_ERR_NULL;
    }
    if (!src && len > 0) {
        return RB_ERR_NULL;
    }

    for (; count < len; count++) {
        next = (rb->head + 1) % rb->capacity;
        if (next == rb->tail) {
            break;
        }
        rb->data[rb->head] = src[count];
        rb->head = next;
    }
    *written = count;
    return RB_OK;
}

rb_status_t rb_read(rb_t *rb, uint8_t *dst, uint32_t len, uint32_t *read_count)
{
    uint32_t count = 0U;
    uint32_t next;
    if (!read_count) {
        return RB_ERR_NULL;
    }
    *read_count = 0U;
    if (!rb || !rb->data) {
        return RB_ERR_NULL;
    }
    if (!dst && len > 0U) {
        return RB_ERR_NULL;
    }

    
    for (; count < len; count++) {
        next = (rb->tail + 1U) % rb->capacity;
        if (rb->head == rb->tail) {
            break;
        }
        dst[count] = rb->data[rb->tail];
        rb->tail = next;
    }
    *read_count = count;
    return RB_OK;

}

rb_status_t rb_peek(const rb_t *rb, uint8_t *dst, uint32_t len, uint32_t *copied)
{
    uint32_t count = 0U;
    uint32_t tail, next;
    if (!copied) {
        return RB_ERR_NULL;
    }
    *copied = 0U;
    if (!rb || !rb->data) {
        return RB_ERR_NULL;
    }
    if (!dst && len > 0U) {
        return RB_ERR_NULL;
    }

    tail = rb->tail;
    for (; count < len; count++) {
        next = (tail + 1) % rb->capacity;
        if (tail == rb->head) {
            break;
        }
        dst[count] = rb->data[rb->tail];
        tail = next;
    }
    *copied = count;
    return RB_OK;
}

rb_status_t rb_discard(rb_t *rb, uint32_t len, uint32_t *discarded)
{
    uint32_t count = 0U;
    uint32_t tail, next;
    if (!discarded) {
        return RB_ERR_NULL;
    }
    *discarded = 0U;
    if (!rb || !rb->data) {
        return RB_ERR_NULL;
    }

    if (rb->capacity == 0U) {
        return RB_ERR_CAPACITY;
    }

    for (; count < len; count++) {
        next = (rb->tail + 1u) % rb->capacity;
        if (tail == rb->head) {
            break;
        }
        tail = next;
    }
    rb->tail = tail;
    *discarded = count;
    return RB_OK;
}