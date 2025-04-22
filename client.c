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


//gcc -o client client.c


int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // ipv4,tcp, default
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr)); // empty template
    serv_addr.sin_family = AF_INET; // force ipv4
    serv_addr.sin_port = htons(atoi(argv[2])); // port number
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr); // serverIP text2bin 

    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); // connect to server
    char recv_buffer[1024]; // received data buffer
    char send_buffer[1024]; // sent data buffer
    int n = recv(sockfd, recv_buffer, sizeof(recv_buffer), 0); // receive data
    printf("received %s\n", recv_buffer); // print received data

    close (sockfd); // cut conn
    return 0;

}