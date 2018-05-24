#ifndef LIBS_MOCKS_CLIENT_SERVER_H_
#define LIBS_MOCKS_CLIENT_SERVER_H_

#include <semaphore.h>

#define HOST "127.0.0.1"
#define PORT 8080

sem_t server_sem;
sem_t client_sem;

char* server_name;
char* client_name;

void client_server_run();

void client_server_show_semaphores();

void client_server_execute_client(int fd_server);

void client_server_execute_server(int fd_client);

#endif
