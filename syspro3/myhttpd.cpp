#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void configServer(int *serverFileDesc, int opt, struct sockaddr_in serverAddress, int serverPort){
  if ((*serverFileDesc = socket(AF_INET, SOCK_STREAM, 0)) == 0){
          perror("socket failed");
          exit(EXIT_FAILURE);
  }
    //Attaching socket to the port specified by the user, or to 9090 if left as default
  // if (setsockopt(*serverFileDesc, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
  //     perror("setsockopt");
  //     exit(EXIT_FAILURE);
  // }

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(serverPort);

  int retVal = bind(*serverFileDesc, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
  if(retVal < 0){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  printf("%d\n", retVal);
}

int main(int argc, char const *argv[]) {
  int servingFD, commandFD, valread;
  int servingPort, commandPort, numOfThreads;
  int opt = 1;
  char *rootDir = (char*)malloc(100*sizeof(char));

  struct sockaddr_in servingAddr, commandAddr;

  /*Checking arguments*/
  if (argc==9) {
    printf("Extended arguments provided by user\n");
    printf("%s\n", argv[2]);
    servingPort = atoi(argv[2]);
    commandPort = atoi(argv[4]);
    numOfThreads = atoi(argv[6]);
    strcpy(rootDir,argv[8]);
  }
  else if (argc == 1) {
    servingPort = 8080;
    commandPort = 9090;
    numOfThreads = 4;
    strcpy(rootDir, "webpages");
    printf("No arguments provided by user. Starting with default arguments.\n servingPort = %d\n commandPort = %d\n numOfThreads = %d\n rootDir = %s\n", servingPort, commandPort, numOfThreads, rootDir);
  }
  /*Finished Checking arguments*/

  /*Configuring socket for the servingPort*/
  configServer(&servingFD, opt, servingAddr, servingPort);
  /*Configuring socket for the commandPort*/
  configServer(&commandFD, opt, commandAddr, commandPort);

  listen(servingFD, 3);
  // if (listen(servingFD, 3) < 0){
  //   perror("listen");
  //   exit(EXIT_FAILURE);
  // }
  if (listen(commandFD, 3) < 0){
    perror("listen");
    exit(EXIT_FAILURE);
  }
  int servingAddrLen = sizeof(servingAddr);
  int commandAddrLen = sizeof(commandAddr);
  int clientSocket, commandSocket;
  printf("waiting...\n" );
  clientSocket = accept(servingFD, (struct sockaddr *)&servingAddr, (socklen_t*)&servingAddrLen);
  commandSocket = accept(commandFD, (struct sockaddr *)&commandAddr, (socklen_t*)&commandAddrLen);
  if (clientSocket<0 || commandSocket<0){
    perror("accept");
    exit(EXIT_FAILURE);
  }
  printf("found?\n" );
  char buffer[1024]={0};
  valread = read( clientSocket , buffer, 1024);
  printf("%s\n",buffer );
  send(clientSocket , "hello" , strlen("hello") , 0 );
  printf("Hello message sent\n");

  /*To find which port the sockets listen to, uncomment this block...
  socklen_t len = sizeof(servingAddr);
  if (getsockname(servingFD, (struct sockaddr *)&servingAddr, &len) == -1)
    perror("getsockname");
  else
    printf("port number %d\n", ntohs(servingAddr.sin_port));

  socklen_t lenc = sizeof(commandAddr);
  if (getsockname(commandFD, (struct sockaddr *)&commandAddr, &lenc) == -1)
    perror("getsockname");
  else
    printf("port number %d\n", ntohs(commandAddr.sin_port));
  */


}
