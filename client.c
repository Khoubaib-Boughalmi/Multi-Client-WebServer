#include "GCS.h"

//GET / HTTP/1.1\r\nHost: www.boobs.com\r\n\r\n
//GET / HTTP/1.1\r\nHost: www.boobs.com\r\n\r\n

int main(void) {
    int sockfd;
    sockfd = createIPV4Socket();
    if (sockfd < 0) {
        perror("socket error");
        return (-1);
    } 
    struct sockaddr_in *addr = createIPV4Address("127.0.0.1", 8080);
    int res = connectSocket(sockfd, addr);
    if (res < 0) {
        perror("connect error");
        return (-1);
    }
    printf("connect success\n");

    char *httpReq = NULL;
    size_t httpReqRead = 0;
    size_t httpReqSize = 0;
    while (1)
    {
        printf("HTTP Request: ");
        httpReqRead = getline(&httpReq, &httpReqSize, stdin);
        if(httpReqRead < 0) {
            perror("getline error");
            break;
        }
        if(!strcmp(httpReq, "exit\n")) {
            printf("adios\n");
            break;
        }
        send(sockfd, httpReq, httpReqRead, 0);
        char httpRes[8192] = "";
        memset(httpRes,  0, sizeof(httpRes));
        int bytesRecv = recv(sockfd, httpRes, sizeof(httpRes), 0);
        if(bytesRecv < 0) {
            perror("recv error");
            break;
        }
        printf("HTTP response: %s\n", httpRes);
        free(httpReq);
        httpReq = NULL;
    }
    close(sockfd);
    free(addr);
    return (0);
}
