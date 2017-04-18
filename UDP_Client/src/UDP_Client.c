// Simple UDP echo server
// Compile with:  gcc -o udpechoserver udpechoserver.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// Max message to echo
#define MAX_MESSAGE	1000

/* server main routine */

int main(int argc, char** argv) {

	// locals
	unsigned short port = 8675; // default port
	int sock; // socket descriptor

	// Was help requested?  Print usage statement
	if (argc > 1 && ((!strcmp(argv[1],"-?"))||(!strcmp(argv[1],"-h"))))
	{
		printf("\nUsage: udpechoserver [-p port] port is the requested \
 port that the server monitors.  If no port is provided, the server \
 listens on port 8675.\n\n");
		exit(0);
	}

	// get the port from ARGV
	if (argc > 1 && !strcmp(argv[1],"-p"))
	{
		if (sscanf(argv[2],"%hu",&port)!=1)
		{
			perror("Error parsing port option");
			exit(0);
		}
	}

	// ready to go
	printf("UDP broadcast client configuring on port: %d\n",port);

	// for UDP, we want IP protocol domain (PF_INET)
	// and UDP transport type (SOCK_DGRAM)
	// no alternate protocol - 0, since we have already specified IP

	if ((sock = socket( PF_INET, SOCK_DGRAM, 0 )) < 0)
	{
		perror("Error on socket creation");
		exit(1);
	}

	// establish address - this is the server and will
	// only be listening on the specified port
	struct sockaddr_in sock_address;

	// address family is AF_INET
	// our IP address is INADDR_ANY (any of our IP addresses)
    // the port number is per default or option above

	sock_address.sin_family = AF_INET;
	sock_address.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_address.sin_port = htons(port);

	// we must now bind the socket descriptor to the address info
	if (bind(sock, (struct sockaddr *) &sock_address, sizeof(sock_address))<0)
	{
		perror("Problem binding");
		exit(0);
	}

	// go into forever loop and echo whatever message is received
	// to console and back to source
	char* buffer = calloc(MAX_MESSAGE,sizeof(char));
	int bytes_read;
    struct sockaddr_in from;
	int from_len;

    while (1) {

		from_len = sizeof(from);

		// read datagram and put into buffer
		bytes_read = recvfrom( sock ,buffer, MAX_MESSAGE,
				0, (struct sockaddr *)&from, &from_len);

		// print info to console
		//printf("Received message from %s port %d\n",
		//	inet_ntoa(from.sin_addr), ntohs(from.sin_port));
		printf("%s",buffer);
		if (bytes_read < 0)
		{
			perror("Error receiving data");
		}

		// clear buffer
		memset(buffer, 0, MAX_MESSAGE);
    }

	// will never get here
	return(0);
}
