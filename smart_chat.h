#ifndef SMART_CHAT_H
#define SMART_CHAT_H

#pragma once
#include <stdio.h>

#define MAX_CLIENTS 1000 // This is actually the higher file descriptor.
#define MAX_NICK_LEN 32
#define SERVER_PORT 7711
    struct client
    {
        int fd;     // Client socket.
        char *nick; // Nickname set by client or NULL.
    };

    /* This global structure encasulates the global state of the chat. */
    struct chatState
    {
        int serversock;                      // Listening server socket.
        int numclients;                      // Number of connected clients right now.
        int maxclient;                       // The greatest 'clients' slot populated.
        struct client *clients[MAX_CLIENTS]; // Clients are set in the corresponding
                                             // slot of their socket descriptor.
    };

class SmartChat
{
public:

    struct chatState *Chat; // Initialized at startup.
    SmartChat();
    ~SmartChat();
    int socketSetNonBlockNoDelay(int fd);
    int createTCPServer(int port);
    int acceptClient(int server_socket);
    void *chatMalloc(size_t size);
    void *chatRealloc(void *ptr, size_t size);
    struct client *createClient(int fd);
    void freeClient(struct client *c);
    void initChat(void);
    void sendMsgToAllClientsBut(int excluded, char *s, size_t len);

private:
};

#endif