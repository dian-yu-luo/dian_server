#include "../http_conn.h"
#define BUFFER_MAX_SIZE (4 * 1012)
// 建立监听事件
int eventListen()
{
    int listenfd;
    // 1. 生成文件描述符
    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);
    // 2. 配置socket 地址
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(8899);
    // 3. 绑定,产生联系
    bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    // 阻塞,开始监听
    listen(listenfd, 5);
    return listenfd;
}
int main(int argc, char const *argv[])
{
    // 连接一共有两种,一种就是无用户连接,一种是确定的有用户连接,由初始化的时候确定
    int listenfd = eventListen();
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);

    char buffer[BUFFER_MAX_SIZE];
    memset(buffer, '\0', BUFFER_MAX_SIZE);
    int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
    recv(connfd, buffer, BUFFER_MAX_SIZE, 0);
    /* ------------------------------------------------------------------- */
    FILE *file = fopen("log.log", "w");
    if (file == NULL)
    {
        perror("无法打开文件");
        return 1;
    }
    fprintf(file, "%s", buffer);
    fclose(file);
    /* ------------------------------------------------------------------- */

    http_conn hc;

    return 0;
}
