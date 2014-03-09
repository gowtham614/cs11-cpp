#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>  // inet_ntop
#include <netdb.h>

#include <stdlib.h>     // exit
#include <string.h>     // memset
#include <stdio.h>      // perror
#include <errno.h>      // perror

#include <iostream> 

// the port client will be connecting to 
#define PORT "3490" 

// max number of bytes we can get at once 
#define MAXDATASIZE 100 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    struct addrinfo hints, *servinfo, *p;
    char dst[INET6_ADDRSTRLEN];
    char buf[MAXDATASIZE];
    int rv, numbytes;
    int sockfd; 
    
    if (argc != 2) 
    {
        std::cerr << "usage: client hostname" << std::endl;
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;      // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets 

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) 
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd < 0) 
        {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) < 0) 
        {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) 
    {
        std::cerr << "client: failed to connect" << std::endl;
        return 2;
    }

    // convert IPv4 and IPv6 addresses from binary to text form
    inet_ntop(p->ai_family, 
              get_in_addr((struct sockaddr *)p->ai_addr),
              dst, sizeof dst);
    
    std::cout << "client: connecting to " << dst << std::endl;

    // all done with this structure
    freeaddrinfo(servinfo); 

    std::cout << "enter message : ";
    if(fgets(buf, MAXDATASIZE, stdin) == NULL)
    {
        std::cerr << "client: failed to get input" << std::endl;
        return 1;
    }

    if (send(sockfd, buf, strlen(buf), 0) == -1)
    {
        perror("send");
        exit(1);
    }

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) 
    {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';
    std::cout << "client: received - " << buf << std::endl;

    close(sockfd);

    return 0;
}
