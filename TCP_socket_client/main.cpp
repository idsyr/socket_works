#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <condition_variable>

using namespace std;

const char* HOST = "127.0.0.1";
const char* PORT = "51720";

int sockfd;
struct sockaddr_in serv_addr;
struct hostent* host;
char buffer[256];
int rw;
mutex mut_buffer;
char temp[256];
char temp2[256];
condition_variable data_cond;

void err(string msg) {close(sockfd); cout<<"ERROR: "+msg<<endl; exit(EXIT_FAILURE);}

void async_user_input(){
    for(;;){
        //cout<<"Enter message: ";
        cin>>temp;
        mut_buffer.lock();
        strncpy(buffer, temp, 256);
        mut_buffer.unlock();
    }
}

void reading(){
    for(;;){
        rw = read(sockfd, temp2, 255); if(rw<0) err("reading socket");
        cout<<temp2<<endl;
    }
}

void writing(){
    for(;;){
        mut_buffer.lock();
        rw = write(sockfd, buffer, strlen(buffer)); if(rw<0)err("writing socket");
        memset(buffer, 0, 256);
        mut_buffer.unlock();
    }

}

int main(int argc, char* argv[]){ cout<<"Client run!"<<endl; //cin>>buffer;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);if(sockfd<0)err("openning socket");
    host = gethostbyname(HOST); if(host==NULL)err("no such host");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr,host->h_addr,host->h_length);
    serv_addr.sin_port = htons(atoi(PORT));

    if(connect(sockfd, (const struct sockaddr*)&serv_addr, sizeof(serv_addr))<0) err("connecting");
    thread t3{async_user_input};
    thread t1{reading};
    thread t2{writing};


    t1.join();
    t2.join();
    t3.join();

    close(sockfd);
    return 0;
}














