//
//  main.c
//  simple-http-server
//
//  Created by blakepark on 2015. 9. 17..
//  Copyright (c) 2015년 blakepark. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define	BACKLOG_SIZE	20
#define BUFFER_SIZE     1024

void error_handle(char* msg){
    printf("%s",msg);
    exit(1);
}

int main(int argc, const char * argv[]) {

    // declare valiables.
    struct sockaddr_in server_address, client_address;
    int server_socket, client_socket;
    int port = 8000;
    FILE *request;

    // init server struct in stack.
    memset(&server_address, 0, sizeof(server_address));

    // set server struct value (http://man7.org/linux/man-pages/man2/socket.2.html)
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);     // change 32bit long intger to network byte order (big endian)
    server_address.sin_family = AF_INET;                    // set address family IPv4
    server_address.sin_port = htons(port);                  // change network byte order

    // create socket to IPv4 internet protocol
    server_socket = socket(PF_INET, SOCK_STREAM, 0);

    // socket bind (error : return -1)
    if (bind(server_socket, (const struct sockaddr *)&server_address , sizeof(server_address)) < 0) error_handle("bind() Error\n");

    while (1) {
        // listen
        if (listen(server_socket, BACKLOG_SIZE) != 1 ) {
            uint client_address_size = sizeof(client_address);

            // accept
            client_socket = accept(server_socket, (struct sockaddr*)&client_address,&client_address_size);
            if (client_socket < 0) error_handle("accept() failed\n");

            // read client request
            request = fdopen(client_socket, "r");

            // malloc request_buffer
            char* request_buffer = malloc(BUFFER_SIZE);
            int request_buffer_size = BUFFER_SIZE;

            // get request
            while (1) {
                char request_inner_buffer[BUFFER_SIZE] = {0};
                ssize_t recv_size = recv(client_socket, request_inner_buffer, BUFFER_SIZE, 0);

                // EOF
                if(recv_size == -1) break;

                // realloc request_buffer_size
                request_buffer_size = request_buffer_size + (int)recv_size;
                request_buffer = realloc(request_buffer, request_buffer_size);

                // concatenate request
                strcat(request_buffer, request_inner_buffer);

                // EOF
                if(recv_size < BUFFER_SIZE) break;
            }


            char response_buffer[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>Hello world!</body></html>";
            send(client_socket,&response_buffer,strlen(response_buffer),0);
            printf("\n==================== response\n%s\n", response_buffer);
            printf("\n");
            printf("\n==================== return_buffer (now_buffer_size : %d)\n%s", request_buffer_size, request_buffer);

            // close
            free(request_buffer);
            fclose(request);
            close(client_socket);
        }
    }
    // close(server_socket);

    return 0;
}
