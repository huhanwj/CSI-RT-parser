#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024

int main(int argc, char** argv){
    int sockfd;
    char buffer[MAXLINE];
    //char *sendmsg;
    struct  sockaddr_in servaddr, cliaddr;

    //Create a socket
    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }   

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    //Server info
    servaddr.sin_family = AF_INET; //ipv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("Socket bind error");
        exit(EXIT_FAILURE);
    }

    int len, n;
    len = sizeof(cliaddr);
    while(1){
        n = recvfrom(sockfd, (char*)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*)&cliaddr, &len);
        if(n>0){
                buffer[n] = '\0';
                printf("Receiving msg from client: %s\n", buffer);
            }
    }
    close(sockfd);
    return 0;
}