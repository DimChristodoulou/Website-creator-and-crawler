#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#define SERVINGPORT 8080
#define COMMANDPORT 9090

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVINGPORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Convert IPv4 and IPv6 addresses from text to binary form
    // if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    // {
    //     printf("\nInvalid address/ Address not supported \n");
    //     return -1;
    // }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
