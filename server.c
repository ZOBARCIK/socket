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
    memset(&hints, 0, sizeof hints); // empty template
    hints.ai_family = AF_INET; // force ipv4 
    hints.ai_socktype = SOCK_STREAM; // tcp
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me

    getaddrinfo (NULL, "5000", &hints, &res); // set up socket for this host(NULL)

        // this part uses the info received from getaddrinfo
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    
    
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    char send_buffer[1024]; // sent data buffer
    char recv_buffer[1024]; // received data buffer
    time_t ticks; // time

    int connfd = accept(sockfd, NULL, NULL);
    if (connfd < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }else{
    printf("Client connected!\n");
    fflush(stdout);
    }

    send(connfd, "handshake\n", strlen("handshake\n"), 0);
    printf("sent: %s\n", "handshake\n");
    fflush(stdout);

    pid_t pid = fork();

    if (pid == 0) {
        while(1) {

            //listen continuously
            int n = recv(connfd, recv_buffer, sizeof(recv_buffer), 0);
            if (n <= 0) {
                printf("Client disconnected or error occurred.\n");
                break;
            }
            recv_buffer[n] = '\0';
            printf(" \n received: %s\n", recv_buffer);
            fflush(stdout);
            pid=1;
            printf(" \n pid: %d\n", pid);
            fflush(stdout);
        }
    }else{
        sleep(1);
        while(1){
            //send hardcoded message
            ticks = time(NULL); // get time
            //snprintf (send_buffer, sizeof send_buffer, "%.24s\r\n", ctime(&ticks)); // format time
            //send (connfd, send_buffer, sizeof send_buffer, 0); // send time
            //
            //char message[]="zarttirizortzort";
            //send (connfd, message, sizeof message,0); // send time


            //message via terminal
            fflush(stdout);
            printf("\n Message to client: ");
            fflush(stdout);
            fgets(send_buffer, sizeof(send_buffer), stdin);
            send(connfd, send_buffer, strlen(send_buffer), 0);

            //eunjung pingpong

            if (strcmp(recv_buffer, "EUN") == 0) {
                send(connfd, "JUNG", strlen("JUNG"), 0);
            } else if (strcmp(recv_buffer, "eun") == 0) {
                send(connfd, "jung", strlen("jung"), 0);
            } else if (strcmp(recv_buffer, "JUNG") == 0) {
                send(connfd, "EUN", strlen("EUN"), 0);
            } else if (strcmp(recv_buffer, "jung") == 0) {
                send(connfd, "eun", strlen("eun"), 0);
            }


            }
        }

    freeaddrinfo(res); // free mem
    close (connfd); // cut conn
    return 0;
}


