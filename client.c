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


    send(sockfd, "handshake recognized\n", strlen("handshake recognized\n"), 0);
    printf("sent: %s\n", "handshake recognized");
    fflush(stdout);
    pid_t pid = fork();
    
    if (pid == 0) {
        
        while (1) { 
            
            //listen continuously
            int n = recv(sockfd, recv_buffer, sizeof(recv_buffer), 0);
            if (n <= 0) break;
            recv_buffer[n] = '\0';
            printf("\n received %s\n", recv_buffer);
            fflush(stdout);
            pid=1;
        }

    }else{
        sleep(1);
        while(1) {
            //input via terminal
            printf("\n Message to server: ");
            fflush(stdout);
            fgets(send_buffer, sizeof(send_buffer), stdin);
            send(sockfd, send_buffer, strlen(send_buffer), 0);

            //eunjung pingpong
            if (strcmp(recv_buffer, "EUN") == 0) {
                send(sockfd, "JUNG", strlen("JUNG"), 0);
            } else if (strcmp(recv_buffer, "eun") == 0) {
                send(sockfd, "jung", strlen("jung"), 0);
            } else if (strcmp(recv_buffer, "JUNG") == 0) {
                send(sockfd, "EUN", strlen("EUN"), 0);
            } else if (strcmp(recv_buffer, "jung") == 0) {
                send(sockfd, "eun", strlen("eun"), 0);
            }

        }
    }
    

    close (sockfd); // cut conn
    return 0;

}