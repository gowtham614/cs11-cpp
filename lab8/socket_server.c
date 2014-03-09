#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>

#include <stdlib.h> // exit
#include <string.h> // memset
#include <stdio.h>  // perror
#include <errno.h>  // perror

#include <iostream> 

// how many pending connections queue will hold  
#define BACKLOG 10     
 
// the port users will be connecting to 
#define PORT "3490"  

// max number of bytes we can get at once
#define MAXDATASIZE 100 
 
// reap all dead processes 
void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) 
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
} 
 
int main(int argc, char * argv[])
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sigaction sa;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    char dst[INET6_ADDRSTRLEN];
    char buf[MAXDATASIZE];
    int i, numbytes;
    int rv, yes = 1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;      // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets
    hints.ai_flags    = AI_PASSIVE;     // use my IP

    // get ready to connect
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) 
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        return 1;
    }
    
    // servinfo now points to a linked list of 1 or more struct addrinfos
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd < 0) 
        {
            perror("server: socket");
            continue;
        }

        // set options on sockets
        // SOL_SOCKET   - manipulate options at the sockets API level
        // SO_REUSEADDR - call should allow reuse of local addresses
        //                socket may bind, except when there is an 
        //                active listening socket bound to the address
        // optval(&yes) - set SO_REUSEADDR on a socket to true
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) 
        {
            perror("setsockopt");
            exit(1);
        }

        // bind the socket to the port we passed in to getaddrinfo():
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) 
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  
    {
        std::cerr << "server: failed to bind" << std::endl;
        return 2;
    }

    // all done with this structure
    freeaddrinfo(servinfo); 
 
    // wait for incoming connections and handle them in some way
    // BACKLOG - the number of connections allowed on the incoming queue
    if (listen(sockfd, BACKLOG) < 0) 
    {
        perror("listen");
        exit(1);
    }
    
    // reap all dead processes
    sa.sa_handler = sigchld_handler; 
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; 
    if (sigaction(SIGCHLD, &sa, NULL) < 0) 
    {
        perror("sigaction");
        exit(1);
    }
    
    std::cout << "server: waiting for connections..." << std::endl;
    
    // main accept() loop
    while(1) 
    {  
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd < 0) 
        {
            perror("accept");
            continue;
        }

        // convert IPv4 and IPv6 addresses from binary to text form
        inet_ntop(their_addr.ss_family, 
                  get_in_addr((struct sockaddr *)&their_addr), 
                  dst, sizeof dst);
        
        std::cout << "server: got connection from " << dst << std::endl;

        if (!fork()) // this is the child process
        { 
            close(sockfd); // child doesn't need the listener
            
            numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0);
            if (numbytes < 0) 
            {
                perror("recv");
                exit(1);
            }
            
            buf[numbytes] = '\0';
            std::cout << "server: received '" << buf << "'" << std::endl;
            
            // converts the sentence into capitals
            for(i=0 ; i < numbytes ; ++i)
            {
                if(isalpha(buf[i]))
                {
                    buf[i] = toupper(buf[i]);
                }
            }
            
            if (send(new_fd, buf, numbytes, 0) < 0)
            {
                perror("send");   
            }
                
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }
 
    return 0;
}

