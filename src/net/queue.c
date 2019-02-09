/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "queue.h"

#include <stddef.h>
#include <string.h>
#include <malloc.h>

queue_object* queue_new(int max_size)
{
        queue_object *queue;
        
        queue = malloc(sizeof(queue_object));
        if (queue == NULL)
                return NULL;
        
        queue->max_size = max_size;
        queue->items_cntr = 0;
        queue->head = NULL;
        queue->tail = NULL;   
        
        return queue;
}

int queue_push(queue_object *queue, void *data, int size, void *source)
{
        queue_item *item;
        void *item_data;
        
        if ((queue->max_size) > 0 && (queue->items_cntr >= queue->max_size))
                return 0;
        
        item = malloc(sizeof(queue_item));
        if (item == NULL)
                return -1;
        
        item_data = malloc(size);
        if (item_data == NULL) {
                free(item);
                return -1;
        }
        
        item->next_item = NULL;
        item->prev_item = NULL;
        item->data = item_data;
        item->size = size;
        item->source = source;
        memcpy(item->data, data, item->size);
        
        if (queue->items_cntr == 0) {
                queue->tail = item;
                queue->head = item;
        } else if (queue->items_cntr == 1) {
                item->next_item = queue->tail;
                queue->tail->prev_item = item;
                
                queue->head = queue->tail;
                queue->tail = item;                
        } else {
                item->next_item = queue->tail;
                queue->tail->prev_item = item;
                
                queue->tail = item;
        }
        
        queue->items_cntr++;
        return 1;
}

void* queue_pop(queue_object *queue, int *size, void **source)
{
        queue_item *item;
        
        if (queue->items_cntr == 0)
                return NULL;
                 
        item = queue->head;
        if (size != NULL)
                *size = item->size;
        if (source != NULL)
                *source = item->source;
        
        if (queue->items_cntr == 1) {
                queue->head = NULL;
                queue->tail = NULL;                
        } else if (queue->items_cntr == 2) {
                queue->head = queue->tail;
                
                queue->head->next_item = NULL;
                queue->head->prev_item = NULL;
        } else {
                queue->head = queue->head->prev_item;
                
                queue->head->next_item = NULL;
        }
        
        queue->items_cntr--;
        return item->data;
}
