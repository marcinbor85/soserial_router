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

#include "transport.h"

#include <string.h>
#include <malloc.h>

transport_object* transport_new(const transport_interface *iface, void *param)
{
        transport_object *obj = malloc(sizeof(transport_object));
        
        if (obj == NULL)
                return NULL;
      
        obj->buf = NULL;
        obj->state = 0;
        obj->iface = iface;
        obj->param = param;
        
        return obj;
}

static void write_nibble(transport_object *obj, uint8_t n)
{
        n &= 0x0F;
        if (n >= 0 && n <= 9) {
                while (obj->iface->write_byte(obj, n + '0', obj->param) <= 0) {};
        } else {
                while (obj->iface->write_byte(obj, (n - 10) + 'A', obj->param) <= 0) {};
        }
}


static void write_byte(transport_object *obj, uint8_t b)
{
        write_nibble(obj, b >> 4);
        write_nibble(obj, b);
}

int transport_send_frame(transport_object *obj, uint8_t *data, int data_size)
{
        uint8_t b;
        uint8_t crc;
        
        if (data_size <= 0 || data_size > 256)
                return -1;
        
        while (obj->iface->write_byte(obj, ':', obj->param) <= 0) {};
        b = data_size - 1;
        write_byte(obj, b);
        crc = b;
        
        while (data_size-- > 0) {
                b = *data++;
                write_byte(obj, b);
                crc += b;
        }
        write_byte(obj, 0x100 - (int)crc);
        
        return 0;
}

static int nibble_to_byte(char ch, uint8_t *b, uint8_t bits)
{
        if (ch >= '0' && ch <= '9') {
                *b |= (uint8_t)((ch - '0') << bits);
        } else if (ch >= 'a' && ch <= 'f') {
                *b |= (uint8_t)((ch - 'a' + 10) << bits);
        } else if (ch >= 'A' && ch <= 'F') {
                *b |= (uint8_t)((ch - 'A' + 10) << bits);
        } else {
                return -1;
        }
        
        return 0;
}

void transport_feed_data(transport_object *obj, char *data, int data_size)
{
        char ch;
        
        while (data_size-- > 0) {
                ch = *data++;
                if (ch == ':') {
                        if (obj->buf != NULL) {
                                free(obj->buf);
                                obj->buf = NULL;
                        }
                        obj->nibble_state = 0;
                        obj->crc = 0;
                        obj->state = 1;
                        continue;
                }
                
                if (obj->state == 0)
                        continue;
                
                if (obj->nibble_state == 0) {
                        obj->byte = 0;                                
                        if (nibble_to_byte(ch, &obj->byte, 4) < 0) {
                                obj->state = 0;
                                continue;
                        }
                        obj->nibble_state = 1;
                        continue;
                } else if (obj->nibble_state == 1) {
                        if (nibble_to_byte(ch, &obj->byte, 0) < 0) {
                                obj->state = 0;
                                continue;
                        }
                        obj->nibble_state = 0;
                } else {
                        obj->nibble_state = 0;
                        continue;
                }
                        
                obj->crc += obj->byte;

                if (obj->state == 1) {
                        obj->frame_size = obj->byte + 1;
                        obj->buf = malloc(obj->frame_size);
                        if (obj->buf == NULL) {
                                obj->state = 0;
                                continue;
                        }                        
                        obj->recv_index = 0;
                        obj->state = 2;
                } else if (obj->state == 2) {
                        obj->buf[obj->recv_index] = obj->byte;
                        if (++obj->recv_index >= obj->frame_size)
                                obj->state = 3;
                } else if (obj->state == 3) {
                        if (obj->crc == 0)
                                obj->iface->recv_frame(obj, obj->buf, obj->frame_size, obj->param);
                        obj->state = 0;
                } else {
                        obj->state = 0;
                }
        }
}

void transport_delete(transport_object *obj)
{
        if (obj == NULL)
                return;
        if (obj->buf != NULL)
                free(obj->buf);
        free(obj);
}
