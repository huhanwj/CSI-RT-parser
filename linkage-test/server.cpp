#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#define MAXLINE 4096
//same as the maximum size of KFIFO buffer size used
#define ERROR -1

int main(int argc, char** argv){
    int listenfd, connectfd;
    struct sockaddr_in servaddr;
    char buff[4096];
    int n;

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("create socket error: ");
        return ERROR;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6666); //remember to match this port to your client

    if(bind(listenfd, (struct  sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        perror("Bind socket error: ");
        return ERROR;
    }

    if(listen(listenfd, 10) == -1){
        perror("Listen to socket error: ");
        return ERROR;
    }
    printf("==========waiting for client=========\n");
    while(true){
        if((connectfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
            perror("Connect failed: ");
            continue;
        }
        while(true){
            n = recv(connectfd, buff, MAXLINE, 0);
            if(n>0){
                buff[n] = '\0';
                printf("Receiving msg from client: %s\n", buff);
            }
        }
        close(connectfd);
    }

    close(listenfd);
    return 0;
}