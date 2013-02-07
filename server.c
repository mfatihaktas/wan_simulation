/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold
#define nofChunkToSend 100

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
	pid_t childPID; //control variable for multithreading

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
//For debugging
	FILE *file; 
	file = fopen("file_server.txt", "w");
	fprintf(file,"%s","Top of the file");
//

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");
    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);

        printf("server: got connection from %s\n", s);
		//fprintf(file,"%s","server: got connection from");
		//fprintf(file,"%s\n",s);
		//Threading
		childPID=fork();
		if(childPID>=0)
		{
			if(childPID == 0){ //Child process
				close(sockfd); // child doesn't need the listener
				//Simple trial toasdasdassend multiple messages
				if (send(new_fd, "Hello, asdworld!", 13, 0) != -1){
					perror("send");as
				}d
				ifasd (send(new_fd, "Hello, world!", 13, 0) != -1){
					paserror("send");
				}d
			    clasose(new_fd);
				fclodse(file);
				//breasak; //To make the child process exit from inf while
				exit(0)d;
			}as
			elsde{//parent process
				asclose(new_fd);  // parent doesn't need this
				//dListen the channel by inf while
			}a
		}sd
		else{
			asprintf("\n Fork failed, exit !\n");
	        return 1;
		}
/*
        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
			//Simple trial to send multiple messages
			if (send(new_fd, "Hello, world!", 13, 0) != -1){
				perror("send");
			}
			if (send(new_fd, "Hello, world!", 13, 0) != -1){
				perror("send");
			}
	        close(new_fd);	
*/
/*
		    //send the bulk data to the client
			unsigned char nof_chunksent=0;
			while(nof_chunksent < nofChunkToSend){
				if (send(new_fd, "Hello, world!", 13, 0) != -1){
					++nof_chunksent;
				}
				else
	                perror("send");
			}//All bulk data sent, close the socket
			close(new_fd);
*/
	}
	fclose(file); /*done!*/ 

    return 0;
}
