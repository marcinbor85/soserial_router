/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   queue.h
 * Author: mborowicz
 *
 * Created on 15 grudnia 2018, 16:39
 */

#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue_item_s {
        void *data;
        int size;
        void *source;
        struct queue_item_s *next_item;
        struct queue_item_s *prev_item;
} queue_item;

typedef struct {
        queue_item *tail;
        queue_item *head;
        int items_cntr;
        int max_size;
} queue_object;

queue_object* queue_new(int max_size);
int queue_push(queue_object *queue, void *data, int size, void *source);
void* queue_pop(queue_object *queue, int *size, void **source);

#endif /* QUEUE_H */

