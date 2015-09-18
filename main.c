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
#define BUFFER_SIZE     2048

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

    // set server struct value .
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // change 32bit long intger to network byte order (big endian)
    server_address.sin_family = AF_INET; // address 패밀리 IPv4
    server_address.sin_port = htons(port); //integer를 네트워크 byte order로 변경

    server_socket = socket(PF_INET, SOCK_STREAM, 0); // http://man7.org/linux/man-pages/man2/socket.2.html IPv4 인터넷 프로토콜

    // socket bind
    if (bind(server_socket, (const struct sockaddr *)&server_address , sizeof(server_address)) < 0) {
        // return -1
        error_handle("bind() Error\n");
    }

    // 계속 listen한다.
    while (1) {

        // listen
        if (listen(server_socket, BACKLOG_SIZE) != 1 ) {
            uint client_address_size = sizeof(client_address);

            // accept
            client_socket = accept(server_socket, (struct sockaddr*)&client_address,&client_address_size);
            if (client_socket < 0) {
                // -1 error
                error_handle("accept() failed\n");
            }

            // request를 읽어온다.
            request = fdopen(client_socket, "r");

            char buffer[BUFFER_SIZE];

            //request 출력
            while (1) {
                printf("==================== request\n");
                ssize_t recv_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
                printf("%s", buffer);

                if(recv_size < 0 || recv_size < BUFFER_SIZE){
                    // 가지고 올게 없으면
                    break;
                }
            }

            char response_buffer[BUFFER_SIZE];
            strcpy(response_buffer,"HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body>Hello world!</body></html>\r\n\r\n");
            send(client_socket,&response_buffer,strlen(response_buffer),0);
            printf("\n==================== response\n%s", response_buffer);
            fclose(request);
            close(client_socket);
        }
    }
    // close(server_socket);

    return 0;
}
