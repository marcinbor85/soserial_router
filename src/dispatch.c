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

#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <malloc.h>

#include "dispatch.h"

#include "net/queue.h"
#include "net/transport.h"

static queue_object *queue;
static pthread_mutex_t queue_mutex;
static pthread_cond_t queue_cond;

static pthread_t consumer_thread;

void dispatch_produce_data(uint8_t *data, int size, void *source)
{
        pthread_mutex_lock(&queue_mutex);

        queue_push(queue, data, size, source);
        
        pthread_cond_signal(&queue_cond);
        pthread_mutex_unlock(&queue_mutex);
}

static void* consumer_thread_handler(void *param)
{
        uint8_t *data;
        int size;
        transport_object *source;
        int i;
        int fd;
        
        while (1) {
                pthread_mutex_lock(&queue_mutex);
                
                if (queue->items_cntr == 0)
                        pthread_cond_wait(&queue_cond, &queue_mutex);
                
                data = queue_pop(queue, &size, (void**)&source);
                pthread_mutex_unlock(&queue_mutex);
                
                if (data != NULL) {
                        
                        fd = *(int*)source->param;
                        i = 0;
                        
                        printf("fd = %d; ", fd);
                        while (i < size)
                                printf("%02X ", data[i++]);
                        puts("");

                        free(data);
                        /*
                        size = security_encode(&buf, data, frame_size, 0x00, 0x1234);
                        if (size > 0) {
                                transport_send_frame(obj, buf, size);

                                size = security_decode(&buf2, buf, size, 0x1234);
                                if (size > 0) {
                                        transport_send_frame(obj, buf2, size);
                                        free(buf2);
                                }

                                free(buf);
                        }*/

                }

        }
}

int dispatch_start(void)
{
        pthread_mutex_init(&queue_mutex, NULL);
        pthread_cond_init(&queue_cond, NULL);

        queue = queue_new(0);
        if (queue == NULL) {
                perror("Could not create dispatch queue");
                return -1;
        }
       
                      
        if (pthread_create(&consumer_thread, NULL, consumer_thread_handler , NULL) < 0) {
                perror("Could not create consumer thread");
                return -1;
        }
       
        return 0;       
}