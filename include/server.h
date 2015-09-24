#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "error.h"

#define BACKLOG_SIZE  2
#define BUFFER_SIZE   1024
#define WORKER_SIZE   20

void *request_routine_cli_sock(void *cli_sock);
void run_server();
