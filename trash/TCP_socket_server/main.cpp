#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <list>

using namespace std;

const char* PORT = "51720";

struct connection{
    int fd;
    int tic_miss;
};

int listenfd;
int connectfd, rw;
char buffer[256];
vector<string> messages1; mutex mut_mes;
struct sockaddr_in serv_addr, cli_addr;
socklen_t clilen;
list<connection> cfds;
vector<connection> ncfds;
mutex mut_ncfds;
mutex mut_cfds;
condition_variable data_cond;

void err(string msg) {close(listenfd); cout<<"ERROR: "+msg<<endl; exit(EXIT_FAILURE);}


void writing(){
    for(;;){
        lock_guard<mutex> gmes{mut_mes};
        lock_guard<mutex> gcfds{mut_cfds};
        for(connection c : cfds){
            rw = write(c.fd, buffer, 256); if(rw<0) err("writing to socket");
        }
    }
}
void reading(int fd){
    for(;;){
        cout<<"new reading thread"<<endl;
        char tecbuffer[256];
        rw = read(fd, tecbuffer, 255); if(rw<0) err("reading from socket");
        mut_mes.lock();
        strncpy(buffer, tecbuffer, 256);
        mut_mes.unlock();
    }
}
void echo_packets(){
    for(;;){
        for(connection c : cfds){
            char tecbuffer[256];
            rw = read(c.fd, tecbuffer, 255); if(rw<0) err("reading from socket");
            if(tecbuffer[0]=='%')continue;
            messages1.push_back(tecbuffer);
        }
        for(connection c : cfds){
            for(string mes : messages1){
                rw = write(c.fd, buffer, 256); if(rw<0) err("writing to socket");
            }
        }
        messages1.clear();
    }
}

///////////////////////////


void echo_packets2(int fd){
    for(;;){

        char tecbuffer[256]; memset(tecbuffer, 0, 256);
        rw = read(fd, tecbuffer, 255); if(rw==0) {cout<<fd<<" error"<<endl;close(fd); terminate();}
        for(connection c : cfds){
            rw = write(c.fd, tecbuffer, 256); if(rw==0) {cout<<c.fd<<" error"<<endl;}
        }

    }
}
void get_connections(){

    for(;;){
        int connectfd = accept(listenfd, (struct sockaddr*) &cli_addr, &clilen); cout<<"new connection"<<endl;
        mut_cfds.lock();
        cfds.push_back(connection{connectfd, 0});
        mut_cfds.unlock();
        thread t5 {echo_packets2, connectfd};
        t5.detach();

    }

}

int main(int argc, char *argv[]){ cout<<"Server run!"<<endl;

    listenfd = socket(AF_INET, SOCK_STREAM, 0); if(listenfd<0) err("opening socket");
    //close(listenfd);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(PORT));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0) err("on binding");
    listen(listenfd, 5);
    clilen = sizeof(cli_addr);
    thread t1{get_connections};

    t1.join();

    close(listenfd);
    close(connectfd);
    return 0;
}


















