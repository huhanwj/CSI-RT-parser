#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "csi_fun.h"

#define BUFSIZE 4096
#define DEFAULT_PORT 6666
int quit;
unsigned char buf_addr[BUFSIZE];
unsigned char data_buf[1500];

COMPLEX csi_matrix[3][3][114];
csi_struct*   csi_status;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        quit = 1;
}
int main(int argc, char* argv[]){
    //FILE*       fp;
    int         fd;
    int         i;
    int         total_msg_cnt,cnt;
    int         log_flag;
    unsigned char endian_flag;
    u_int16_t   buf_len;
    int         sockfd;
    int         n;
    struct sockaddr_in servaddr;

    log_flag = 1;
    csi_status = (csi_struct*)malloc(sizeof(csi_struct));
    /* check usage */
    if (argc < 3){
        /* If you want to log the CSI for off-line processing,
         * you need to specify the name of the output file
         */
        log_flag  = 0;
        printf("/**************************************/\n");
        printf("/*   Usage: recv_csi <target ip address>   */\n");
        printf("/**************************************/\n");
    }
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Create socket error: ");
        return errno;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(DEFAULT_PORT); //should be matched with server side
    if (argc == 3){
        if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
        perror("inet_pton error: ");
        return errno;
        }
    }
    if (argc > 3){
        printf(" Too many input arguments !\n");
        return errno;
    }
    if(connect(sockfd, (struct  sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("Connect error: ");
        return errno;
    }
    fd = open_csi_device();
    if (fd < 0){
        perror("Failed to open the device...");
        return errno;
    }
    printf("#Receiving data and sending them out! Press Ctrl+C to quit!\n");
    quit = 0;
    total_msg_cnt = 0;
     while(1){
        if (1 == quit){
            return 0;
            close(sockfd);
            close_csi_device(fd);
        }

        /* keep listening to the kernel and waiting for the csi report */
        cnt = read_csi_buf(buf_addr,fd,BUFSIZE);

        if (cnt){
            total_msg_cnt += 1;

            /* fill the status struct with information about the rx packet */
            record_status(buf_addr, cnt, csi_status);

            /* 
             * fill the payload buffer with the payload
             * fill the CSI matrix with the extracted CSI value
             */
            record_csi_payload(buf_addr, csi_status, data_buf, csi_matrix); 
            
            printf("Recv %dth msg with rate: 0x%02x | payload len: %d\n",total_msg_cnt,csi_status->rate,csi_status->payload_len);
            if (log_flag){
                buf_len = csi_status->buf_len;
                if(send(sockfd, &buf_len, strlen(&buf_len), 0) < 0){
                    perror("Send buffer length error: ");
                    return errno;
                }
                if(send(sockfd, buf_addr, buf_len, 0) < 0){
                    perror("Send buffer data error: ");
                    return errno;
                }
            }
        }
    }
    close(sockfd);
    close_csi_device(fd);
    free(csi_status);
    return 0;
}