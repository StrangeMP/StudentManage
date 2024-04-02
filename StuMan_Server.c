#include "StuMan_Server.h"
#include "StuMan_Handler.h"
#include "StuMan_Memory.h"
#include <process.h> // For _beginthreadex
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

bool should_terminate = false;

void handle_client(void *client_socket) {
    SOCKET client = *(SOCKET *)client_socket;
    // Receive the request
    char buffer[65536];
    int bytes_received = recv(client, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        {
            FILE *f = fopen("__tmp.txt", "w");
            fputs(buffer, f);
            fclose(f);
        }
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
                if (response)
                    send(client, response, strlen(response), 0);
                else {
                    char invalidReq[] = "HTTP/1.1 405 Method Not Allowed\r\n"
                                        "Content-Length: 0\r\n"
                                        "Access-Control-Allow-Origin: *\r\n"
                                        "Connection: close\r\n\r\n";
                    send(client, invalidReq, strlen(invalidReq), 0);
                }

                FREE(response);
            }
        }
    }

    // Close the client socket
    closesocket(client);
    _endthreadex(0);
}

void server_thread(void *unused) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        return;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        fprintf(stderr, "Error creating socket.\n");
        WSACleanup();
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Error binding socket.\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(stderr, "Error listening.\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    printf("Server listening on port 8080...\n");

    while (!should_terminate) {
        SOCKET client_socket = accept(server_socket, NULL, NULL);
        if (client_socket != INVALID_SOCKET) {
            _beginthreadex(NULL, 0, handle_client, &client_socket, 0, NULL);
        }
    }

    closesocket(server_socket);
    WSACleanup();
    _endthreadex(0);
}

void RunServer() {
    _beginthreadex(NULL, 0, server_thread, NULL, 0, NULL);

    getchar();

    return;
}
