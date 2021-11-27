#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define MAXLINE 4096 
#define ERROR -1

int main(int argc, char** argv){
    int sockfd, n;
    char recvline[4096];
    struct sockaddr_in servaddr;

    if (argc != 2){
        printf("usage: ./client <ipaddress>\n");
        return ERROR;
    }
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Create socket error: ");
        return ERROR;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666); //should be matched with server side
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
        perror("inet_pton error: ");
        return ERROR;
    }

    if(connect(sockfd, (struct  sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("Connect error: ");
        return ERROR;
    }
    int msg_count = 0;
    
    while(true){
        char sendline[] = "This is a test message!\n";
        printf("send %d msg to server: %s\n",msg_count+1, sendline);
        if(send(sockfd, sendline, strlen(sendline), 0) < 0){
            perror("Send msg error: ");
            return ERROR;
        }
        msg_count++;
        usleep(102400);
        if(msg_count > 10000){
            close(sockfd);
            break;
        }
    }
    return 0;
}