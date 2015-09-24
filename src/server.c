#include "server.h"

void run_server() {
    // declare server valiables.
  struct sockaddr_in server_address, client_address;
  int server_socket, client_socket;
  int port = 8000;

  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);     // change 32bit long intger to network byte order (big endian)
  server_address.sin_family = AF_INET;                    // set address family IPv4
  server_address.sin_port = htons(port);                  // change network byte order
  server_socket = socket(PF_INET, SOCK_STREAM, 0);        // create socket to IPv4 internet protocol

//    printf("\n\n\n\n\n\n\n================= bind ===============\n");
  if (bind(server_socket, (const struct sockaddr *)&server_address , sizeof(server_address)) < 0) error_handle("bind() Error\n");

  while (1) {
//    printf("\n\n\n\n\n\n\n================= listen ===============\n");
    if (listen(server_socket, BACKLOG_SIZE) != 1 ) {

      pthread_t p_thread[WORKER_SIZE];
      uint client_address_size = sizeof(client_address);

//      printf("================= accept ===============\n");
      client_socket = accept(server_socket, (struct sockaddr*)&client_address,&client_address_size);
      int random_worker = rand() % (WORKER_SIZE + 1);
      printf("random_worker : %d\n",random_worker);

      // return -> success : 0, error : -1
      if (pthread_create(&p_thread[random_worker], NULL, request_routine_cli_sock, (void *)&client_socket) < 0) error_handle("pthread_create() error");
      pthread_detach(p_thread[random_worker]);
    }
  }
 // close(server_socket);
}
  

void *request_routine_cli_sock(void *cli_sock){
//    printf("================= created thread ===============\n");
  FILE *request;
  int client_socket;
  char *request_buffer;
  int request_buffer_size;

  client_socket = *((int *) cli_sock);              // read client socket
  request = fdopen(client_socket, "r");             // read client request
  request_buffer = malloc(BUFFER_SIZE);             // init request_buffer in heap
  if (request_buffer == NULL) error_handle("malloc() NULL");
  request_buffer_size = BUFFER_SIZE;

  // get request
  while (1) {
    char request_inner_buffer[BUFFER_SIZE] = {0};
    ssize_t recv_size = recv(client_socket, request_inner_buffer, BUFFER_SIZE, 0);

    // EOF
    if(recv_size == -1) break;

    // realloc request_buffer_size
    request_buffer_size = request_buffer_size + (int)recv_size;
    request_buffer = realloc(request_buffer, request_buffer_size);
    if (request_buffer == NULL) error_handle("realloc() NULL");

    // concatenate request
    strcat(request_buffer, request_inner_buffer);

    // EOF
    if(recv_size < BUFFER_SIZE) break;
  }

  // send response to client
  char response_buffer[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>Hello world!</body></html>";
  send(client_socket,&response_buffer,strlen(response_buffer),0);

//    printf("================= request ================= (now_buffer_size : %d)\n%s\n", request_buffer_size, request_buffer);
//    printf("================= response =================\n%s\n", response_buffer);

  free(request_buffer);
  fclose(request);

//    sleep(10);  // release test
  return cli_sock;
}
