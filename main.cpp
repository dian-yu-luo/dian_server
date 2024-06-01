#include <iostream>
#include <dian_log.h>
#include <smart_chat.h>
#include <unistd.h>
int main(int argc, char const *argv[])
{
    DianLog dianlog;
    dianlog.logRedLine();

    SmartChat smartchat;
    smartchat.initChat();
    while (1)
    {
        int c = smartchat.acceptClient(smartchat.Chat->serversock);
        if (c == -1)
            continue;
        printf("Client accepted FD %d\n", c);
        sleep(1);
    }

    dianlog.logRedLine();
    return 0;
}
