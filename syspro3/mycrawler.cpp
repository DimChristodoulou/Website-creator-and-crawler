#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

using namespace std;

int main(int argc, char const *argv[]) {

  char *nameOrIpAddress = (char*)malloc(1024*sizeof(char));
  char *saveDir = (char*)malloc(1024*sizeof(char));
  char *startingURL = (char*)malloc(1024*sizeof(char));
  int serverPort, commandPort, numOfThreads;

  thread *threadPool = (thread)malloc(numOfThreads*sizeof(thread));

  /*Checking arguments*/
  if (argc==9) {
    printf("Extended arguments provided by user\n");
    strcpy(nameOrIpAddress,argv[2]);
    serverPort = atoi(argv[4]);
    commandPort = atoi(argv[6]);
    numOfThreads = atoi(argv[8]);
    strcpy(saveDir,argv[10]);
    strcpy(startingURL,argv[11]);
  }
  else
    printf("Incorrect number of arguments. Please execute the program in this way:\n ./mycrawler -h host_or_IP -p port -c command_port -t num_of_threads -d save_dir starting_URL\n Exiting...\n" );
  /*Finished Checking arguments*/

}
