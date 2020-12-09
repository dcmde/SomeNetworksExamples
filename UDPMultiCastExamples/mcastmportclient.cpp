#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <poll.h>

struct sockaddr_in localSock;
struct ip_mreq group;
int datalen;
char databuf[1024];
const int port[] = {4321, 4322, 4323};

using namespace std;

int main(int argc, char *argv[]) {
    // socket descriptors
    int sds[3];
    struct pollfd structpollfd[3];

    if (argc < 3) {
        std::cout << "Please enter : group_ip local_ip" << std::endl;
        return 0;
    }

    const char *gourpIp = argv[1];
    const char *localIp = argv[2];

    //Create datagram sockets on which to receive on each port.
    for (int & sd : sds) {
        sd = socket(AF_INET, SOCK_DGRAM, 0);

        if (sd < 0) {
            perror("Opening datagram socket error");
            exit(1);
        }
        else {
            printf("Opening datagram socket....OK.\n");
        }
    }

    // Enable SO_REUSEADDR to allow multiple instances of this
    // application to receive copies of the multicast datagrams.
    int reuse = 1;
    for (int sd : sds) {
        if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(reuse)) < 0) {
            perror("Setting SO_REUSEADDR error");
            exit(1);
        }
        else {
            printf("Setting SO_REUSEADDR...OK.\n");
        }
    }

    // Bind to the proper port number with the IP address
    // specified as INADDR_ANY.
    memset((char *) &localSock, 0, sizeof(localSock));
    localSock.sin_family = AF_INET;
    localSock.sin_addr.s_addr = INADDR_ANY;

    for (int i = 0; i < 3; ++i) {
        localSock.sin_port = htons(port[i]);
        if (bind(sds[i], (struct sockaddr *) &localSock, sizeof(localSock))) {
            perror("Binding datagram socket error");
            exit(1);
        }
        else {
            printf("Binding datagram socket...OK.\n");
        }
    }

    // Join the multicast group on the local interface.
    // Note that this IP_ADD_MEMBERSHIP option must be
    // called for each local interface over which the multicast
    // datagrams are to be received.

    group.imr_multiaddr.s_addr = inet_addr(gourpIp);
    group.imr_interface.s_addr = inet_addr(localIp);

    for (int sd : sds) {
        if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &group, sizeof(group)) < 0) {
            perror("Adding multicast group error");
            exit(1);
        }
        else {
            printf("Adding multicast group...OK.\n");
        }
    }
    // Read from the socket.
    datalen = sizeof(databuf);

    // Set the pollfd structures with file descriptors and requested events of data being available to read.
    for (int i = 0; i < 3; ++i) {
        structpollfd[i].fd = sds[i];
        structpollfd[i].events = POLLIN;
    }

    while (1) {

        // Block until an event occur.
        if (poll(structpollfd, 3, -1) < 0) {
            perror("Poll error");
            exit(1);
        }

        // Determine which file has data to be read.
        for (int i = 0; i < 3; ++i) {
            
            if (structpollfd[i].revents == POLLIN) {

                if (read(structpollfd[i].fd, databuf, datalen) < 0) {
                    perror("Reading datagram message error");
                    exit(1);
                }
                else {
                    std::cout << "Reading datagram message...OK." << std::endl;
                    printf("The message from multicast server is: \"%s\"\n", databuf);
                }
            }
        }
    }
    return 0;
}