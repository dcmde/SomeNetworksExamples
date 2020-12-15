/*
 * listen to the given group on the given port.
 */
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <poll.h>

struct sockaddr_in localAddr, publisherAddr;
struct ip_mreq group;
struct pollfd pollfd_struct;
socklen_t publisherAddr_size;
struct timeval read_timeout;
int sd;
int datalen;
char databuf[1024];

using namespace std;

int main(int argc, char *argv[]) {

    if (argc < 3) {
        std::cout << "Please enter : group_ip port" << std::endl;
        return 0;
    }

    const char *gourpIp = argv[1];
    int port = atoi(argv[2]);

    // Create a datagram socket on which to receive.
    sd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sd < 0) {
        perror("Opening datagram socket error");
        exit(1);
    }
    else {
        printf("Opening datagram socket....OK.\n");
    }

    int reuse = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(reuse)) < 0) {
        perror("Setting SO_REUSEADDR error");
        close(sd);
        exit(1);
    }
    else {
        printf("Setting SO_REUSEADDR...OK.\n");
    }


    // Disable IP_MULTICAST_ALL to get only messages related to the given IP group.
    int mc_all = 0;
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_ALL, (char *) &mc_all, sizeof(mc_all)) < 0) {
        perror("Setting IP_MULTICAST_ALL error");
        close(sd);
        exit(1);
    }

    memset((char *) &localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(port);
    localAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sd, (struct sockaddr *) &localAddr, sizeof(localAddr)) < 0) {
        perror("Binding datagram socket error");
        close(sd);
        exit(1);
    }
    else {
        printf("Binding datagram socket...OK.\n");
    }

    group.imr_multiaddr.s_addr = inet_addr(gourpIp);
    group.imr_interface.s_addr = INADDR_ANY;

    if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &group, sizeof(group)) < 0) {
        perror("Adding multicast group error");
        close(sd);
        exit(1);
    }
    else {
        printf("Adding multicast group...OK.\n");
    }

    // Read from the socket.
    datalen = sizeof(databuf);
    publisherAddr_size = sizeof(publisherAddr);

    memset(&pollfd_struct, 0, sizeof(pollfd_struct));
    pollfd_struct.fd = sd;
    pollfd_struct.events = POLLIN;

    while (1) {

        if (poll(&pollfd_struct, 1, -1) < 0) {
            perror("Polling error");
            close(sd);
            exit(1);
        }
        else {
            printf("Poll returned ... OK.\n");
        }

        if (pollfd_struct.revents == POLLIN) {
            if (recvfrom(sd, databuf, datalen, 0, (struct sockaddr *) &publisherAddr, &publisherAddr_size) < 0) {
                perror("Reading datagram message error");
                close(sd);
                exit(1);
            }
            else {
                printf("The message is : %s from ip %s and port %u\n", databuf, inet_ntoa(publisherAddr.sin_addr),
                       ntohs(publisherAddr.sin_port));
            }
        }
    }
    return 0;
}