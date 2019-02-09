/*
The MIT License (MIT)

Copyright (c) 2018 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <stdint.h>
#include <stddef.h>

struct transport_object_s;

typedef int (*transport_write_byte_cb)(struct transport_object_s *obj, char byte, void *param);
typedef void (*transport_recv_frame_cb)(struct transport_object_s *obj, uint8_t *data, int data_size, void *param);

typedef struct {
        transport_write_byte_cb write_byte;
        transport_recv_frame_cb recv_frame;
} transport_interface;

typedef struct transport_object_s {
        uint8_t *buf;
        void *param;
       
        uint8_t byte;
        uint8_t crc;
        
        int recv_index;
        int frame_size;
        
        int state;
        int nibble_state;
        
        transport_interface const *iface;
} transport_object;

transport_object* transport_new(const transport_interface *iface, void *param);
int transport_send_frame(transport_object *obj, uint8_t *data, int data_size);
void transport_feed_data(transport_object *obj, char *data, int data_size);
void transport_delete(transport_object *obj);

#endif /* TRANSPORT_H */

