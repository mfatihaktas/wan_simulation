/*
** Producer will send all the data generated to DSA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "4000" // the port client will be connecting to 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

#define MAXSENDLENGTH 100
#define MAXRECVLENGTH 100
int nofChunksToSend=0;
int sockfd;
FILE *file; //For debugging purposes
//Some buffers necessary for communication
char send_buf[MAXSENDLENGTH];
char recv_buf[MAXSENDLENGTH];
char temp_buffer[10];

//Method pre_decs
int sendTo(int socketFD, char* send_buf);
int recvFrom(int socketFD);
void close_();
int
main(int argc, char *argv[])
{
//	int sockfd;
    int numbytes;  
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 3) {
        fprintf(stderr,"usage: client hostname, # of chunks to be received\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("producer: connecting to gateway: %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
	
	//Communicate with the gateway
	char* t_argv[]={"A",argv[2]};
	sendTo(sockfd, formCommPacket("DataSendSessionInfo", t_argv));
	recvFrom(sockfd); //Wait for OK message
	if(strcmp(recv_buf, "OK")!=0){ //OK is not received
		printf("OK message is not recved\n");
		exit(1);
	}
	printf("'OK' message is received successfully \n");

	file = fopen("file_p.txt", "w");
	fprintf(file,"%s","Top of the file");
	//Send the bulk data to the gateway
	int nofChunksToSend = atoi(argv[2]); //No control assume a number is arged
	unsigned int nof_chunksent=0;
	while(nof_chunksent < nofChunksToSend){
		strcpy(send_buf, "Hello World birader");
		sprintf(temp_buffer, "%d", nof_chunksent);
		strcat(send_buf, temp_buffer);
		//printf("send_buf: %s\n", send_buf);
		//send send_buf to receiver
		if (sendTo(sockfd, send_buf) == 1){
			++nof_chunksent;
		}
		memset(send_buf, 0, MAXSENDLENGTH);//Initialize for the next chunk sending session
		memset(temp_buffer, 0, 10);
	}//All bulk data sent, close the socket
    close(sockfd);
	fclose(file);
	//exit(0);
    return 0;
}
//Based on the comm message type create and return the message
char* formCommPacket(char* type, char** argv){
	int packet_l;
	char* commpacket; 
	if(strcmp(type, "DataSendSessionInfo")==0){ //
		packet_l=strlen(argv[0])+strlen(argv[1])+strlen("Session:#ofChunks")+1;
		commpacket = malloc(packet_l*sizeof(char));
		strcpy(commpacket, "Session:");
		strcat(commpacket, argv[0]);
		strcat(commpacket, "#ofChunks");
		strcat(commpacket, argv[1]);
	}
	else{
		printf("Type is not recognized by formCommPacket()\n");
		exit(1);
	}
	return commpacket;
}
void close_()
{
	close(sockfd); //Socket will be closed by the server when all of the data is sent
	fclose(file);
}

int sendTo(int socketFD, char* send_buf){
	if (send(socketFD, send_buf, (int)strlen(send_buf), 0) != -1){
		return 1; //sent successfully
	}
	else{
        perror("send");
		return -1; //could not be sent
	}
}
//
int recvFrom(int socketFD){
	int numbytes = recv(socketFD, recv_buf, MAXRECVLENGTH, 0); //No flags are necessary
	if(numbytes>0){//Successful reception
		return numbytes;
	}	
	else if (numbytes == 0) {//socket is closed by the other side
		close_();
	    exit(1);
	}
	else{ //Unsuccessful reception
		perror("recv");
		return -1;
	}
}

