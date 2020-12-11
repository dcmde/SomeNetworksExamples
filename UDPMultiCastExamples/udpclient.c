#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_LEN 1024

int main(int argc, char** argv) {

    int port = 1234;
    int sockfd, sfd;
    struct sockaddr_in serverAddr;
    char buffer[BUF_LEN];

    sockfd = socket(PF_INET, SOCK_DGRAM, 0); // Set a UDP socket.

    // Create UDP interface.
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    //int reuse = 1;
    //setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *) &reuse, sizeof(reuse));

    strcpy(buffer, "Hello Server\n");
    int len = sendto(sockfd, buffer, BUF_LEN, 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    printf("Requested %i sent %i\n", BUF_LEN, len);

    printf("Data sent: %s", buffer);

    printf("to port %i and address %s\n", ntohs(serverAddr.sin_port), inet_ntoa(serverAddr.sin_addr));

    close(sockfd);

    return 0;
}