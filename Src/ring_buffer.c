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
#include "ring_buffer.h"

bool lock;

struct ring_buffer_t{
    size_t head;
    size_t tail;
	size_t buffer_size;
    uint8_t* data;
    bool is_empty;
    bool is_full;
};


rbuf_handle_t ring_buffer_init(size_t size)
{

	rbuf_handle_t buf = malloc(sizeof(ring_buffer_t));
	if (buf == NULL)
	{
		return NULL;
	}

	buf->head = 0;
	buf->tail = 0;
	buf->buffer_size = size;
	buf->data = calloc(buf->buffer_size, sizeof(*buf->data));
	buf->is_empty = true;
	buf->is_full = false;
	lock = false;
	
	return buf;
}

ring_buffer_SIG ring_buffer_flush(rbuf_handle_t handle)
{
	if (lock)
	{
		return BUSY;
	}
	lock = true;
	if (handle == NULL)
	{
		lock = false;
		return FAIL;
	}
	handle->head = 0;
	handle->tail = 0;
	handle->is_empty = true;
	handle->is_full = false;
	lock = false;
	return OK;	
}

size_t ring_buffer_available_bytes(rbuf_handle_t handle)
{
	if (lock)
	{
		return 0;
	}
	lock = true;
	if (handle == NULL)
	{
		lock = false;
		return 0;
	}

	if (handle->is_empty)
	{
		lock = false;
		return 0;
	}
	else if (handle->is_full)
	{
		lock = false;
		return (handle->buffer_size);
	}
	else if (handle->head > handle->tail)
	{
		size_t ret = (size_t)(handle->head - handle->tail);
		lock = false;
		return ret;
	}
	else
	{		
		/*This case shouldn't happen normally since the tail should never exceed the head*/
		size_t ret = (size_t)(handle->buffer_size + handle->head - handle->tail);
		lock = false;
		return ret;
	}
}


ring_buffer_SIG ring_buffer_write(rbuf_handle_t handle, uint8_t* data, size_t size)
{
	if (lock)
	{
		return BUSY;
	}
	lock = true;
	if (handle == NULL)
	{
		lock = false;
		return FAIL;
	}
	size_t buf_unused_space;
	if (handle->is_full)
	{
		lock = false;
		return FAIL;
	}
	else if (handle->is_empty)
	{
		buf_unused_space = handle->buffer_size;
	}
	else if (handle->head > handle->tail)
	{
		buf_unused_space = handle->buffer_size - (handle->head - handle->tail);
	}
	else
	{
		buf_unused_space = handle->tail - handle->head;
	}

	if (buf_unused_space < size)
	{
		lock = false;
		return FAIL;
	}

	for (size_t i = 0; i < size; i++)
	{
		handle->data[handle->head] = *(data + i);
		handle->head = ((handle->head + 1) % handle->buffer_size);
	}
	if (handle->head == handle->tail)
	{
		handle->is_full = true;
	}
	handle->is_empty = false;
	lock = false;
	return OK;
}

ring_buffer_SIG ring_buffer_read(rbuf_handle_t handle, uint8_t* data, size_t size)
{
	if (lock)
	{
		return BUSY;
	}
	lock = true;
	if (handle == NULL)
	{
		lock = false;
		return FAIL;
	}
	size_t buf_size_used;
	if (handle->is_empty)
	{
		lock = false;
		return FAIL;
	}
	else if (handle->is_full)
	{
		buf_size_used = handle->buffer_size;
	}
	else if (handle->head > handle->tail)
	{
		buf_size_used = (handle->head - handle->tail);
	}
	else
	{
		buf_size_used = handle->buffer_size - (handle->tail - handle->head);
	}

	if (buf_size_used > size)
	{
		lock = false;
		return FAIL;
	}


	for (size_t i = 0; i < buf_size_used; i++)
	{
		*(data + i) = handle->data[handle->tail];
		handle->data[handle->tail] = '\0';
		handle->tail = ((handle->tail + 1) % handle->buffer_size);	
	}
	if (handle->tail == handle->head)
	{
		handle->is_empty = true;
	}
	handle->is_full = false;
	lock = false;
	return OK;
}


ring_buffer_SIG ring_buffer_deinit(rbuf_handle_t handle)
{
	if (lock)
	{
		return BUSY;
	}
	lock = true;
	if (handle == NULL)
	{
		lock = false;
		return FAIL;
	}
	free(handle->data);
	free(handle);
	lock = false;
	return OK;
}