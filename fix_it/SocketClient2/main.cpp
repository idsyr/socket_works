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

const char* HOST = "127.0.0.1";
const char* PORT = "51720";

int sockfd;

void err(string msg) {close(sockfd); const char* add = "ERROR: "; perror(add); perror(msg.c_str()); exit(EXIT_FAILURE);}

int main(int argc, char* argv[]){

    struct sockaddr_in serv_addr;
    struct hostent* host;
    char buffer[256];
    int rw;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);if(sockfd<0)err("openning socket");
    host = gethostbyname(HOST); if(host==NULL)err("no such host");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr,host->h_addr,host->h_length);
    serv_addr.sin_port = htons(atoi(PORT));

    if(connect(sockfd, (const struct sockaddr*)&serv_addr, sizeof(serv_addr))<0) err("connecting");

    printf("Enter message"); memset(buffer, 0, 256); fgets(buffer, 255, stdin);
    rw = write(sockfd, buffer, strlen(buffer)); if(rw<0)err("writing socket"); memset(buffer, 0, 256);
    rw = read(sockfd, buffer, 255); if(rw<0) err("reading socket"); printf("%s", buffer);

    close(sockfd);
    return 0;
}














