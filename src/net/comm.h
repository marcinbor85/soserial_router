/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   comm.h
 * Author: mborowicz
 *
 * Created on 13 grudnia 2018, 10:52
 */

#ifndef COMM_H
#define COMM_H

typedef struct {
        
} comm_interface;

typedef struct {
        
} comm_object;

comm_object* comm_new(comm_interface *iface);
void comm_feed_data(comm_interface *obj, uint8_t *data, int data_size);
int comm_send_request(comm_interface *obj, uint8_t *data, int data_size);


#endif /* COMM_H */

