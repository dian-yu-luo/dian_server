#include "smart_chat.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
SmartChat::SmartChat()
{
}

SmartChat::~SmartChat()
{
}

int SmartChat::socketSetNonBlockNoDelay(int fd)
{
    int flags, yes = 1;

    /* Set the socket nonblocking.
     * Note that fcntl(2) for F_GETFL and F_SETFL can't be
     * interrupted by a signal. */
    if ((flags = fcntl(fd, F_GETFL)) == -1)
        return -1;
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        return -1;

    /* This is best-effort. No need to check for errors. */
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));
    return 0;
}

int SmartChat::createTCPServer(int port)
{
    int s;
    struct sockaddr_in sa;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return -1;

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr *)&sa, sizeof(sa)) == -1 ||
        listen(s, 511) == -1)
    {
        close(s);
        return -1;
    }
    return s;
}

int SmartChat::acceptClient(int server_socket)
{
    int s;

    while (1)
    {
        struct sockaddr_in sa;
        socklen_t slen = sizeof(sa);
        s = accept(server_socket, (struct sockaddr *)&sa, &slen);
        if (s == -1)
        {
            if (errno == EINTR)
                continue; /* Try again. */
            else
                return -1;
        }
        break;
    }
    return s;
}

void *SmartChat::chatMalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        perror("Out of memory");
        exit(1);
    }
    return ptr;
}

void *SmartChat::chatRealloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    if (ptr == NULL)
    {
        perror("Out of memory");
        exit(1);
    }
    return ptr;
}

client *SmartChat::createClient(int fd)
{
    struct client *c = (struct client *)chatMalloc(sizeof(*c));
    socketSetNonBlockNoDelay(fd); // Pretend this will not fail.
    c->fd = fd;
    c->nick = NULL;
    assert(Chat->clients[c->fd] == NULL); // This should be available.
    Chat->clients[c->fd] = c;
    /* We need to update the max client set if needed. */
    if (c->fd > Chat->maxclient)
        Chat->maxclient = c->fd;
    Chat->numclients++;
    return c;
}

void SmartChat::freeClient(client *c)
{
    free(c->nick);
    close(c->fd);
    Chat->clients[c->fd] = NULL;
    Chat->numclients--;
    if (Chat->maxclient == c->fd)
    {
        /* Ooops, this was the max client set. Let's find what is
         * the new highest slot used. */
        int j;
        for (j = Chat->maxclient - 1; j >= 0; j--)
        {
            if (Chat->clients[j] != NULL)
                Chat->maxclient = j;
            break;
        }
        if (j == -1)
            Chat->maxclient = -1; // We no longer have clients.
    }
    free(c);
}

void SmartChat::initChat(void)
{
    Chat = (struct chatState *)chatMalloc(sizeof(*Chat));
    memset(Chat, 0, sizeof(*Chat));
    /* No clients at startup, of course. */
    Chat->maxclient = -1;
    Chat->numclients = 0;

    /* Create our listening socket, bound to the given port. This
     * is where our clients will connect. */
    Chat->serversock = createTCPServer(SERVER_PORT);
    if (Chat->serversock == -1)
    {
        perror("Creating listening socket");
        exit(1);
    }
}

void SmartChat::sendMsgToAllClientsBut(int excluded, char *s, size_t len)
{
    for (int j = 0; j < Chat->maxclient; j++)
    {
        if (Chat->clients[j] == NULL ||
            Chat->clients[j]->fd == excluded)
            continue;

        /* Important: we don't do ANY BUFFERING. We just use the kernel
         * socket buffers. If the content does not fit, we don't care.
         * This is needed in order to keep this program simple. */
        write(Chat->clients[j]->fd, s, len);
    }
}
