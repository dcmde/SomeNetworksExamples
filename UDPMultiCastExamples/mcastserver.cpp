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

    if (argc < 4) {
        std::cout << "Please enter : group_ip local_ip port" << std::endl;
        return 0;
    }

    const char *groupIp = argv[1];
    const char *localIp = argv[2];
    int port = atoi(argv[3]);

    /* Create a datagram socket on which to send. */
    try{
        sd = socket(AF_INET, SOCK_DGRAM, 0);

        if (sd < 0) {
            perror("Opening datagram socket error");
            throw std::invalid_argument("Error");
        }
    }
    catch (std::invalid_argument& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    // Initialize the group sockaddr structure
    memset((char *) &groupSock, 0, sizeof(groupSock));
    groupSock.sin_family = AF_INET;
    groupSock.sin_addr.s_addr = inet_addr(groupIp);
    groupSock.sin_port = htons(port);

    /* Disable loopback so you do not receive your own datagrams.*/
    /*
    char loopch = 0;
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *) &loopch, sizeof(loopch)) < 0) {
        perror("Setting IP_MULTICAST_LOOP error");
        close(sd);
        exit(1);

    }
    else
        printf("Disabling the loopback...OK.\n");
    */
    /* Set local interface for outbound multicast datagrams. */
    /* The IP address specified must be associated with a local, */
    /* multicast capable interface. */

    localInterface.s_addr = inet_addr(localIp);
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *) &localInterface, sizeof(localInterface)) < 0) {
        perror("Setting local interface error");
        exit(1);
    }
    else
        printf("Setting the local interface...OK\n");

    /* Send a message to the multicast group specified by the*/
    /* groupSock sockaddr structure. */
    /*int datalen = 1024;*/
    if (sendto(sd, databuf, datalen, 0, (struct sockaddr *) &groupSock, sizeof(groupSock)) < 0) {
        perror("Sending datagram message error");
    }
    else
        printf("Sending datagram message...OK\n");

    /* Try the re-read from the socket if the loopback is not disable
    if(read(sd, databuf, datalen) < 0)
    {
    perror("Reading datagram message error\n");
    close(sd);
    exit(1);
    }
    else
    {
    printf("Reading datagram message from client...OK\n");
    printf("The message is: %s\n", databuf);
    }
    */

    return 0;
}