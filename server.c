#include <sys/socket.h> //socket
#include <netinet/in.h> //sockaddr_in 
#include <arpa/inet.h>  //inet_addr
#include <stdio.h>  
#include <stdlib.h> 
#include <unistd.h> //unix std
#include <errno.h>  //?
#include <string.h> //strerror
#include <sys/types.h>  //?
#include <time.h>   // chrono time
#include <netdb.h> //getaddrinfo



//gcc -o server server.c



int main(int argc, char *argv[]) {


    int listenfd = socket (AF_INET, SOCK_STREAM, 0); // ipv4,tcp, default

    struct addrinfo hints, *res; 
    memset (¾hints, 0, sizeof hints)); // empty template
    hints.ai_family = AF_INET; // force ipv4 
    hints.ai_socktype = SOCK_STREAM; // tcp
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me

    getaddrinfo (NULL, "5000", &hints, &res); // set up socket for this host(NULL)

        // this part uses the info received from getaddrinfo
    int sockfd = socket (res->ai_family, res->ai_socktype, res->ai_protocol); // create socket
    bind (sockfd, res->ai_addr, res->ai_addrlen); // bind to port



    // listen and send data

    listen (sockfd, 10); // listen socket (10 max client)

    char send_buffer[1024]; // sent data buffer
    char recv_buffer[1024]; // received data buffer
    time_t ticks; // time

    while(1) {

        int connfd= accept (sockfd, NULL, NULL);
        ticks = time(NULL); // get time
        snprintf (send_buffer, sizeof send_buffer, "%.24s\r\n", ctime(&ticks)); // format time
        send (connfd, send_buffer, sizeof send_buffer, 0); // send time
    }

    freeaddrinfo(res); // free mem
    close (connfd); // cut conn
    return 0;
}


    