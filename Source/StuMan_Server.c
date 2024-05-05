#include "StuMan_Server.h"
#include "StuMan_Handler.h"
#include "StuMan_Log.h"
#include "StuMan_Memory.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
    #include <netinet/in.h>
    #include <pthread.h>
    #include <sys/socket.h>
    #include <unistd.h>
typedef int Socket_T;
typedef ssize_t Recv_T;
#else
    #include <process.h> // For _beginthreadex
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
typedef int Recv_T;
typedef SOCKET Socket_T;
#endif

#define MAX_CLIENTS 32
bool should_terminate = false;

#ifdef __linux
void *handle_client(void *client_socket)
#else
unsigned int handle_client(void *client_socket)
#endif
{
    Socket_T client = *(Socket_T *)client_socket;
    // Receive the request
    char buffer[65536];
    Recv_T bytes_received = recv(client, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        char first_word[10];
        sscanf(buffer, "%s", first_word);
        if (strcmp(first_word, "OPTIONS") == 0) {
            char PreFlight_Req[] = "HTTP/1.1 200 OK\r\n"
                                   "Access-Control-Allow-Origin: *\r\n"
                                   "Access-Control-Allow-Methods: POST, OPTIONS\r\n"
                                   "Access-Control-Allow-Headers: Authorization, Content-Type, "
                                   "Access-Control-Allow-Origin\r\n\r\n";
            send(client, PreFlight_Req, strlen(PreFlight_Req), 0);
        } else {
            char *request_body = strstr(buffer, "\r\n\r\n");
            if (request_body) {
                request_body += 4;
                printf("Received request body: %s\n", request_body);

                // Process the request and get the response
                char *response = Handler(request_body);

                // Send the HTTP response
                // send(client_socket, http_response, strlen(http_response), 0);
                if (response) {
                    send(client, response, strlen(response), 0);
                    FREE(response);
                } else {
                    char invalidReq[] = "HTTP/1.1 405 Method Not Allowed\r\n"
                                        "Content-Length: 0\r\n"
                                        "Access-Control-Allow-Origin: *\r\n"
                                        "Connection: close\r\n\r\n";
                    send(client, invalidReq, strlen(invalidReq), 0);
                }
                Log(request_body);
            }
        }
    }

// Close the client socket
#ifdef __linux__
    close(client);
    pthread_exit(NULL);
#else
    closesocket(client);
    _endthreadex(0);
#endif
    return 0;
}

#ifdef __linux__
void *server_thread(void *unused) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        fprintf(stderr, "Error creating socket.\n");
        return NULL;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        fprintf(stderr, "Error binding socket.\n");
        close(server_socket);
        return NULL;
    }

    if (listen(server_socket, SOMAXCONN) == -1) {
        fprintf(stderr, "Error listening.\n");
        close(server_socket);
        return NULL;
    }

    printf("\n\n\nServer listening on port 8080...\n");

    // Array to store thread handles (adjust MAX_CLIENTS as needed)
    pthread_t client_threads[MAX_CLIENTS];
    int num_client_threads = 0;

    while (!should_terminate) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket != -1) {
            pthread_create(&client_threads[num_client_threads], NULL, handle_client,
                           &client_socket);
            num_client_threads++;
        }
    }

    // Wait for all client handling threads to finish
    for (int i = 0; i < num_client_threads; i++) {
        pthread_join(client_threads[i], NULL);
    }

    close(server_socket);
    pthread_exit(NULL);
}
#else
unsigned int server_thread(void *unused) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        return 1;
    }

    Socket_T server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        fprintf(stderr, "Error creating socket.\n");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Error binding socket.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(stderr, "Error listening.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("\n\n\nServer listening on port 8080...\n");

    // Array to store thread handles (adjust MAX_CLIENTS as needed)
    HANDLE client_threads[MAX_CLIENTS];
    int num_client_threads = 0;

    while (!should_terminate) {
        Socket_T client_socket = accept(server_socket, NULL, NULL);
        if (client_socket != INVALID_SOCKET) {
            client_threads[num_client_threads] =
                (HANDLE)_beginthreadex(NULL, 0, handle_client, &client_socket, 0, NULL);
            num_client_threads++;
        }
    }

    // Wait for all client handling threads to finish
    for (int i = 0; i < num_client_threads; i++) {
        WaitForSingleObject(client_threads[i], INFINITE);
        CloseHandle(client_threads[i]);
    }

    closesocket(server_socket);
    WSACleanup();
    _endthreadex(0);
    return 0;
}
#endif

void RunServer() {
#ifdef __linux
    pthread_t serverThread;
    pthread_create(&serverThread, NULL, server_thread, NULL);
#else
    _beginthreadex(NULL, 0, server_thread, NULL, 0, NULL);
#endif
    getchar();
    should_terminate = true;
    return;
}
