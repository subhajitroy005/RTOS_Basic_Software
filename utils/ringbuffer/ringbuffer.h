/*
 * ringbuffer.h
 *
 *  Created on: 2014骞�11鏈�27鏃�
 *      Author: johnson
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <string.h>
#include <inttypes.h>


#define USBRxBufferSize 					1024

/* ring buffer */
struct ringbuffer {
	uint8_t *buffer_ptr;
	/* use the msb of the {read,write}_index as mirror bit. You can see this as
	 * if the buffer adds a virtual mirror and the pointers point either to the
	 * normal or to the mirrored buffer. If the write_index has the same value
	 * with the read_index, but in differenct mirro, the buffer is full. While
	 * if the write_index and the read_index are the same and within the same
	 * mirror, the buffer is empty. The ASCII art of the ringbuffer is:
	 *
	 *          mirror = 0                    mirror = 1
	 * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
	 * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Full
	 * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
	 *  read_idx-^                   write_idx-^
	 *
	 * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
	 * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Empty
	 * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
	 * read_idx-^ ^-write_idx
	 *
	 * The tradeoff is we could only use 32KiB of buffer for 16 bit of index.
	 * But it should be enough for most of the cases.
	 *
	 * Ref: http://en.wikipedia.org/wiki/Circular_buffer#Mirroring */
	uint16_t read_mirror :1;
	uint16_t read_index :15;
	uint16_t write_mirror :1;
	uint16_t write_index :15;
	/* as we use msb of index as mirror bit, the size should be signed and
	 * could only be positive. */
	int16_t buffer_size;
};

enum ringbuffer_state {
	RT_RINGBUFFER_EMPTY, RT_RINGBUFFER_FULL, RT_RINGBUFFER_HALFFULL /* half full is neither full nor empty */
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn void ringbuffer_init(struct ringbuffer*, uint8_t*, int16_t)
 * @brief
 *
 * @pre
 * @post
 * @param rb
 * @param pool
 * @param size
 */
void ringbuffer_init(struct ringbuffer *rb, uint8_t *pool, int16_t size);

/**
 * @fn uint32_t ringbuffer_put(struct ringbuffer*, const uint8_t*, uint16_t)
 * @brief
 *
 * @pre
 * @post
 * @param rb
 * @param ptr
 * @param length
 * @return
 */
uint32_t ringbuffer_put(struct ringbuffer *rb, const uint8_t *ptr,
		uint16_t length);

/**
 * @fn uint32_t ringbuffer_put_force(struct ringbuffer*, const uint8_t*, uint16_t)
 * @brief
 *
 * @pre
 * @post
 * @param rb
 * @param ptr
 * @param length
 * @return
 */
uint32_t ringbuffer_put_force(struct ringbuffer *rb, const uint8_t *ptr,
		uint16_t length);

/**
 * @fn uint32_t ringbuffer_putchar(struct ringbuffer*, const uint8_t)
 * @brief
 *
 * @pre
 * @post
 * @param rb
 * @param ch
 * @return
 */
uint32_t ringbuffer_putchar(struct ringbuffer *rb, const uint8_t ch);

/**
 * @fn uint32_t ringbuffer_putchar_force(struct ringbuffer*, const uint8_t)
 * @brief
 *
 * @pre
 * @post
 * @param rb
 * @param ch
 * @return
 */
uint32_t ringbuffer_putchar_force(struct ringbuffer *rb, const uint8_t ch);

/**
 * @fn uint32_t ringbuffer_get(struct ringbuffer*, uint8_t*, uint16_t)
 * @brief
 *
 * @pre
 * @post
 * @param rb
 * @param ptr
 * @param length
 * @return
 */
uint32_t ringbuffer_get(struct ringbuffer *rb, uint8_t *ptr, uint16_t length);

/**
 * @fn uint32_t ringbuffer_getchar(struct ringbuffer*, uint8_t*)
 * @brief
 *
 * @pre
 * @post
 * @param rb
 * @param ch
 * @return
 */
uint32_t ringbuffer_getchar(struct ringbuffer *rb, uint8_t *ch);

/**
 * @fn void ringbuffer_flush(struct ringbuffer*)
 * @brief
 *
 * @pre
 * @post
 * @param rb
 */
void ringbuffer_flush(struct ringbuffer *rb);

/**
 * @fn uint16_t ringbuffer_get_size(struct ringbuffer*)
 * @brief
 *
 * @pre
 * @post
 * @param rb
 * @return
 */
uint16_t ringbuffer_get_size(struct ringbuffer *rb);

/**
 * @fn enum ringbuffer_state ringbuffer_status(struct ringbuffer*)
 * @brief
 *
 * @pre
 * @post
 * @param rb
 * @return
 */
enum ringbuffer_state ringbuffer_status(struct ringbuffer *rb);

/** return the size of data in rb */
/**
 * @fn uint16_t ringbuffer_data_len(struct ringbuffer*)
 * @brief
 *
 * @pre
 * @post
 * @param rb
 * @return
 */
uint16_t ringbuffer_data_len(struct ringbuffer *rb);
/** return the size of empty space in rb */
/**
 * @def ringbuffer_empty_space
 * @brief
 *
 */
#define  ringbuffer_empty_space(rb) ((rb)->buffer_size -  ringbuffer_data_len(rb))

///** return the size of empty space in rb */
//#define RT_RINGBUFFER_EMPTY(rb) ((rb)->buffer_size - RT_RINGBUFFER_SIZE(rb))
#ifdef __cplusplus
}
#endif

#endif /* RINGBUFFER_H_ */
