/*
 * listen to the given group on the given port.
 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <iostream>

struct sockaddr_in localSock;
struct ip_mreq group;
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

    memset((char *) &localSock, 0, sizeof(localSock));
    localSock.sin_family = AF_INET;
    localSock.sin_port = htons(port);
    localSock.sin_addr.s_addr = INADDR_ANY;

    if (bind(sd, (struct sockaddr *) &localSock, sizeof(localSock)) < 0) {
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
    while (1) {
        if (read(sd, databuf, datalen) < 0) {
            perror("Reading datagram message error");
            close(sd);
            exit(1);
        }
        else {
            printf("Reading datagram message...OK.\n");
            printf("The message from multicast server is: \"%s\"\n", databuf);
        }
    }
    return 0;
}