/*
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Please enter a port to listen to.\n");
        return 0;
    }

    int port = atoi(argv[1]);
    int sockfd, bind_flag, msg_len;
    struct sockaddr_in si_me, si_other;
    struct ip_mreq ip_mcast;
    char buffer[1024];
    socklen_t addr_size;

    // Create UDP socket.
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockfd < 0){
        printf("Error socket().\n");
        close(sockfd);
        return 0;
    }

    // Set interface on local IP with given port.
    memset(&si_me, '\0', sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket and the interface to receive incoming messages.
    bind_flag = bind(sockfd, (struct sockaddr *) &si_me, sizeof(si_me));

    if(bind_flag < 0){
        printf("Error bind().\n");
        close(sockfd);
        return 0;
    }

    addr_size = sizeof(si_other);

    while (1) {
        // get the message and the information of the sender.
        msg_len = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *) &si_other, &addr_size);

        if (msg_len < 0) {
            printf("Receive from error.\n");
            return 1;
        }

        // Print data and connection info.
        printf("Data Received: %s ", buffer);
        printf("from port %i and address %s\n", ntohs(si_other.sin_port), inet_ntoa(si_other.sin_addr));
    }

    setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const char *) &ip_mcast, sizeof(ip_mcast));
    close(sockfd);
    return 0;
}