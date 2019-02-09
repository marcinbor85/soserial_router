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

#include "net/transport.h"
#include "net/security.h"
#include "dispatch.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

static int write_byte(transport_object *obj, char byte, void *param)
{
        int s;
        int fd = *(int*)param;
        s = write(fd, &byte, 1);
        return s;
}

static void read_frame(transport_object *obj, uint8_t *data, int frame_size, void *param)
{
        dispatch_produce_data(data, frame_size, obj);             
}

static const transport_interface tport_iface = {
        .write_byte = write_byte,
        .recv_frame = read_frame
};

static void *server_thread_handler(void *socket_desc)
{
	int sock = *(int*)socket_desc;
	int size;
	char buf[64];
        
        transport_object *tport = transport_new(&tport_iface, socket_desc);

        if (tport == NULL) {
                perror("Could not create transport protocol object");
                return NULL;
        }
        
	while ((size = recv(sock, buf, 64, 0)) > 0)
                transport_feed_data(tport, buf, size);

        puts("Client disconnected");

        transport_delete(tport);
	free(socket_desc);
	
	return NULL;
}

int server_start(int port)
{
	int socket_desc, client_sock, c, *new_sock;
	struct sockaddr_in server, client;
        uint8_t *b;
        pthread_t server_thread;
	
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc < 0) {
		perror("Could not create socket");
                return -1;
	}
	puts("Socket created");
	
        memset(&server, 0, sizeof(struct sockaddr_in));
                
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	server.sin_port = htons(port);
	
	if (bind(socket_desc, (struct sockaddr *)&server , sizeof(server)) < 0) {
		perror("Bind failed");
		return -1;
	}
	puts("Bind done");
	
	listen(socket_desc, 4);
	
        b = (uint8_t*)&server.sin_addr.s_addr;
	fprintf(stdout, "Waiting for incomming connection on %d.%d.%d.%d:%d\n", b[0], b[1], b[2], b[3], port);
	c = sizeof(struct sockaddr_in);
	while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {
                
                b = (uint8_t*)&client.sin_addr;
                fprintf(stdout, "Connection accepted from %d.%d.%d.%d:%d\n", b[0], b[1], b[2], b[3], client.sin_port);

		new_sock = malloc(sizeof(int));
		*new_sock = client_sock;
                                
		if (pthread_create(&server_thread, NULL, server_thread_handler , (void*)new_sock) < 0) {
			perror("Could not create server thread");
                        
                        free(new_sock);
			return -1;
		}
                puts("Server thread created");
	}
	
	if (client_sock < 0) {
		perror("Accept failed");
		return -1;
	}
	
	return 0;
}



