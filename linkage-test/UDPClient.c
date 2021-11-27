#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 8080
#define MAXLINE 1024

int main(int argc, char** argv){
    int sockfd, n;
    char recvline[4096];
    struct sockaddr_in servaddr;

    if (argc != 2){
        printf("usage: ./client <ipaddress>\n");
        exit(EXIT_FAILURE);
    }
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Create socket error: ");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666); //should be matched with server side
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
        perror("inet_pton error: ");
        exit(EXIT_FAILURE);
    }

    int msg_count = 0;
    int len = sizeof(servaddr);
    while(1){
        char *sendline = "This is a test message from CSI1!\n";
        printf("send %d msg to server: %s\n",msg_count+1, sendline);
        sendto(sockfd, (const char*) sendline, strlen(sendline), MSG_CONFIRM, (const struct sockaddr*) &servaddr, len);
        msg_count++;
        usleep(102400);
        if(msg_count > 10000){
            close(sockfd);
            break;
        }
    }
    return 0;
}