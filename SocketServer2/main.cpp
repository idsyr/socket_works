#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <cstring>

using namespace std;

const char* PORT = "51720";

int listenfd;

void err(string msg) {close(listenfd); const char* add = "ERROR: "; perror(add); perror(msg.c_str()); exit(EXIT_FAILURE);}

int main(int argc, char *argv[]){

    int connectfd, rw;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    listenfd = socket(AF_INET, SOCK_STREAM, 0); if(listenfd<0) err("opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(PORT));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0) err("on binding");
    listen(listenfd, 5);
    clilen = sizeof(cli_addr);
    connectfd = accept(listenfd, (struct sockaddr*) &cli_addr, &clilen);

    memset(buffer, 0, 256);
    rw = read(connectfd, buffer, 255); if(rw<0) err("reading from socket");
    printf("Here is he message: %s", buffer);
    rw = write(connectfd, "I got your message", 18); if(rw<0) err("writing to socket");

    close(listenfd);
    close(connectfd);
    return 0;
}
