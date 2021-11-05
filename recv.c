#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>

#include "csi_fun.h"

#define BUFSIZE 4096
unsigned char buf_addr[BUFSIZE];
unsigned char data_buf[1500];
int quit;
COMPLEX csi_matrix[3][3][114];
csi_struct*   csi_status;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        quit = 1;
}
int main(int argc, char* argv[])
{
    FILE*       fp;
    int         fd;
    int         i;
    int         total_msg_cnt,cnt;
    int         log_flag;
    unsigned char endian_flag;
    u_int16_t   buf_len;
    int         listenfd;
    int         connectfd;
    struct      sockaddr_in servaddr;
    char        buff[BUFSIZE];
    int         n;
    log_flag = 1;
    csi_status = (csi_struct*)malloc(sizeof(csi_struct));
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perrno("create socket errno: ");
        return errno;
    }
    if (1 == argc){
        /* If you want to log the CSI for off-line processing,
         * you need to specify the name of the output file
         */
        log_flag  = 0;
        printf("/**************************************/\n");
        printf("/*   Usage: recv <output_file>    */\n");
        printf("/**************************************/\n");
    }
    if (2 == argc){
        fp = fopen(argv[1],"w");
        if (!fp){
            printf("Fail to open <output_file>, are you root?\n");
            fclose(fp);
            return 0;
        }   
    }
    if (argc > 2){
        printf(" Too many input arguments !\n");
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6666); //remember to match this port to your client

    if(bind(listenfd, (struct  sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        perrno("Bind socket errno: ");
        return errno;
    }

    if(listen(listenfd, 10) == -1){
        perrno("Listen to socket errno: ");
        return errno;
    }
    printf("==========Waiting for CSI stream=========\n");
    while(1){
        if((connectfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
            perror("Connect failed: ");
            continue;
        }
        while(1){
            n = recv(connectfd, buff, BUFSIZE, 0);
            if(n > 0 && n < 3){
                buff[n] = '\0';
                fwrite(buff,1,2,fp);
            }
            else{
                buff[n] = '\0';
                fwrite(buff,1,strlen(buff),fp);
            }
        }
        close(connectfd);
    }
    fclose(fp);
    close_csi_device(fd);
    free(csi_status);
    close(listenfd);
    return 0;
}