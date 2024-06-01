#ifndef SMART_CHAT_H
#define SMART_CHAT_H

#pragma once

class SmartChat
{
public:
    SmartChat();
    ~SmartChat();
    int socketSetNonBlockNoDelay(int fd);
    int createTCPServer(int port);
    int acceptClient(int server_socket);
private:
};

#endif