#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "externals/tt_logger/tt_logger.h"

#define DEFAULT_PORT 4444
#define FLAG_OFF 0
#define FLAG_ON 1

char buffer[1024];
int daemonize_flag = FLAG_OFF;
int print_flag = FLAG_OFF;
int client_index = 0;

void strcat_c (char *str, char c)
{
    for (;*str;str++); // note the terminating semicolon here.
    *str++ = c;
    *str++ = 0;
}

void command_output()
{
    FILE *fpipe;
    char c = 0;
    if (0 == (fpipe = (FILE*)popen(buffer, "r")))
    {
       perror("popen() failed.");
    }
//    bzero(buffer, 1024);
    while (fread(&c, sizeof c, 1, fpipe))
    {
           strcat_c(buffer, c);
    }
    pclose(fpipe);
}

int daemonize()
{
    pid_t   pid;

    pid = fork();
    if (pid > 0) exit(EXIT_SUCCESS);
    if (pid < 0) printf("Can't fork\n");
    return(pid);
}

int main(int argc, char **argv)
{
    int c;
    int portno = DEFAULT_PORT;
    while( (c = getopt(argc, argv, "dh:")) != -1) {
        switch(c) {
            case 'd':
                daemonize_flag = FLAG_ON;
                break;
            case 'h':
                portno = atoi(optarg);
        }
    }
    if(daemonize_flag == FLAG_ON) {
        daemonize();
    }
    tt_init(1, "tt_server.log");
    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Server Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portno);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    tt_log(1, "%s", "Binding");
    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0) {
        printf("[-]Error in binding.\n");
        tt_log(3 ,"Binding failed");
        exit(1);
    }
    printf("[+]Bind to port\n");

    if(listen(sockfd, 10) == 0) {
        printf("[+]Listening....\n");
    }
    else {
        printf("[-]Error in binding.\n");
    }


    while(1) {
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if(newSocket < 0) {
            exit(1);
            tt_log(3 ,"%s", "Error on accepting");
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        tt_log(1, "Connection accepted from %s:%d: Client number %d", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port), ++client_index);
        if((childpid = fork()) == 0) {
            close(sockfd);

            while(1) {
                recv(newSocket, buffer, 1024, 0);
                if(buffer[0] == '\n') {
                    print_flag = FLAG_ON;
                }
                buffer[strlen(buffer)-1] = '\0';
                if(print_flag == FLAG_OFF) {
                    tt_log(1 ,"%s %d %s %s", "Getting the message from client ", client_index, " :", buffer);
                }
                if(strncmp(buffer, "quit", 4) == 0) {
                    file_close();
                    tt_log(1, "%s %d %s", "Client: ", client_index, "  is quiting");
                    break;
                }
                else {
                    command_output();
                    send(newSocket, buffer, 1024, 0);
                    print_flag = FLAG_OFF;
                    bzero(buffer, sizeof(buffer));
                }
            }
        }

    }

    close(newSocket);
    return 0;
}
