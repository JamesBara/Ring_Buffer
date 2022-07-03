/*
*MIT License
*
*Copyright (c) 2022 Dimitris Baradakis
*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in all
*copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*SOFTWARE.
*/

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ring_buffer_t ring_buffer_t;
typedef ring_buffer_t* rbuf_handle_t;

typedef enum ring_buffer_SIG
{
	OK,
	FAIL,
	BUSY
}ring_buffer_SIG;


rbuf_handle_t ring_buffer_init(size_t size);
size_t ring_buffer_available_bytes(rbuf_handle_t handle);
ring_buffer_SIG ring_buffer_flush(rbuf_handle_t handle);
ring_buffer_SIG ring_buffer_write(rbuf_handle_t handle, uint8_t* data, size_t size);
ring_buffer_SIG ring_buffer_read(rbuf_handle_t handle, uint8_t* data, size_t size);
ring_buffer_SIG ring_buffer_deinit(rbuf_handle_t handle);


#ifdef __cplusplus
}
#endif
#endif /*RING_BUFFER_H_*/

