/* Server Send Multicast Datagram code example. */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <unistd.h>

struct in_addr localInterface;
struct sockaddr_in groupSock;
int sd;
char databuf[1024] = "Multicast test message lol!";
int datalen = sizeof(databuf);


int main(int argc, char *argv[]) {

    if (argc < 3) {
        std::cout << "Please enter : group_ip port" << std::endl;
        return 0;
    }

    const char *groupIp = argv[1];
    int port = atoi(argv[2]);

    try {
        sd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sd < 0) {
            perror("Opening datagram socket error");
            throw std::invalid_argument("Error");
        }
    } catch (std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    memset((char *) &groupSock, 0, sizeof(groupSock));
    groupSock.sin_family = AF_INET;
    groupSock.sin_addr.s_addr = inet_addr(groupIp);
    groupSock.sin_port = htons(port);

    char loopch = 1;
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *) &loopch, sizeof(loopch)) < 0) {
        perror("Setting IP_MULTICAST_LOOP error");
        close(sd);
        exit(1);
    }
    else {
        printf("Disabling the loopback...OK.\n");
    }

    localInterface.s_addr = INADDR_ANY;
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *) &localInterface, sizeof(localInterface)) < 0) {
        perror("Setting local interface error");
        exit(1);
    }
    else {
        printf("Setting the local interface...OK\n");
    }

    if (sendto(sd, databuf, datalen, 0, (struct sockaddr *) &groupSock, sizeof(groupSock)) < 0) {
        perror("Sending datagram message error");
    }
    else {
        printf("Sending datagram message...OK\n");
    }

    return 0;
}