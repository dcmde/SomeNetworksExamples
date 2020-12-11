#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv) {


    int port = 4000;
    int sockfd;
    struct sockaddr_in si_me, si_other;
    struct ip_mreq ip_mcast;
    char buffer[1024];
    socklen_t addr_size;

    // Create UDP socket.
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Set interface on local IP with given port.
    memset(&si_me, '\0', sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = INADDR_ANY;

    // Set socket options for multicast listening.
    ip_mcast.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
    ip_mcast.imr_interface.s_addr = INADDR_ANY;
    setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *) &ip_mcast, sizeof(ip_mcast));

    // Bind the socket and the interface to receive incoming messages.
    bind(sockfd, (struct sockaddr *) &si_me, sizeof(si_me));
    addr_size = sizeof(si_other);

    while (1) {
        // get the message and the information of the sender.
        recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *) &si_other, &addr_size);

        // Print data and connection info.
        printf("Data Received: %s", buffer);
        printf("from port %i and address %s\n", ntohs(si_other.sin_port), inet_ntoa(si_other.sin_addr));
    }
    setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const char *) &ip_mcast, sizeof(ip_mcast));
    close(sockfd);
    return 0;
}