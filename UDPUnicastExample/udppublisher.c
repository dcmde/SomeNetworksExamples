/*
 * Send a message to the given IP address on the given port.
 * The publisher interface is not bind to a port so the communication cannot be reused by the listener.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_LEN 1024

int main(int argc, char **argv) {

    if (argc != 4) {
        printf("Please give : ip port word .\n");
        return 0;
    }

    int port = atoi(argv[2]);
    const char* listener_ip = argv[1];
    int sockfd, msg_len;
    struct sockaddr_in listenerAddr;
    char buffer[BUF_LEN];

    strcpy(buffer,argv[3]);

    sockfd = socket(PF_INET, SOCK_DGRAM, 0); // Set a UDP socket.

    // Create UDP interface.
    memset(&listenerAddr, '\0', sizeof(listenerAddr));
    listenerAddr.sin_family = AF_INET;
    listenerAddr.sin_port = htons(port);


    if(inet_aton(listener_ip,&listenerAddr.sin_addr) < 0){
        printf("Error inet_aton()");
        return 0;
    }

    msg_len = sendto(sockfd, buffer, BUF_LEN, 0, (struct sockaddr *) &listenerAddr, sizeof(listenerAddr));

    if(msg_len != BUF_LEN){
        printf("Warning sendto(), complete message was not sent.\n");
    }
    close(sockfd);
    return 0;
}