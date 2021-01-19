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
struct sockaddr_in group;
int sd;
char databuf[1024] = {0};
int datalen = sizeof(databuf);

int main(int argc, char *argv[]) {

    if (argc < 3) {
        std::cout << "Please enter : ip_group port" << std::endl;
        return 0;
    }

    int cpt = 0;
    for(int i = 3; i < argc; ++i){
	for(int j=0; j < strlen(argv[i]); ++j){
	    databuf[cpt] = argv[i][j];
            cpt++;
 	}
        databuf[cpt] = ' ';
    cpt++;
    }

   for(int i = 0; i < 20; ++i){
      printf("%c", databuf[i]);
   }

   printf("\n");

    const char *ip_group = argv[1];
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

    memset((char *) &group, 0, sizeof(group));
    group.sin_family = AF_INET;
    group.sin_port = htons(port);

    if (inet_aton(ip_group, &group.sin_addr) < 0) {
        std::cerr << "Error inet_aton()" << std::endl;
    }

    // The flag has to be changed depending on the application.
    char loopch = 1;
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *) &loopch, sizeof(loopch)) < 0) {
        perror("Setting IP_MULTICAST_LOOP error");
        close(sd);
        exit(1);
    }

    localInterface.s_addr = INADDR_ANY;
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *) &localInterface, sizeof(localInterface)) < 0) {
        perror("Setting local interface error");
        exit(1);
    }
    else {
        printf("Setting the local interface...OK\n");
    }

    if (sendto(sd, databuf, datalen, 0, (struct sockaddr *) &group, sizeof(group)) < 0) {
        perror("Sending datagram message error");
    }
    else {
        printf("Sending datagram message...OK\n");
    }

    return 0;
}
