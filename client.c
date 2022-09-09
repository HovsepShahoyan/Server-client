#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 4444
#define FLAG_ON 1
#define FLAG_OFF 0

int main(int argc, char **argv)
{
    int clientSocket, ret;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    int opt;
    int portno = DEFAULT_PORT;
    char* inet_address = "127.0.0.1";
    while((opt = getopt(argc, argv, "i:h:")) != -1) {
        switch(opt) {
            case 'h':
                     portno = atoi(optarg);
                break;
            case 'i':
                    strncpy(inet_address, optarg, sizeof(optarg));
               break;
        }
    }
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Client Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portno);
    serverAddr.sin_addr.s_addr = inet_addr(inet_address);

    ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Connected to Server.\n");

    while(1){
        printf("$");
        fgets(buffer, 1024, stdin);
        send(clientSocket, buffer, 1024, 0);

        if(strncmp(buffer, "quit", 4) == 0){
            printf("[-]Disconnected from server.\n");
            close(clientSocket);
            exit(1);
        }

        if(recv(clientSocket, buffer, 1024, 0) < 0){
            printf("[-]Error in receiving data.\n");
        }
        printf("%s", buffer);
    }

    return 0;
}















































































































































































































































