#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
int main(int argc, char * argv[]) {
	struct sockaddr_in server_address, return_address;
	char server_ip[] = "127.0.0.1";
	int port = 12000;
	char message[] = "Test from client";
	int socket_id = socket(AF_INET, SOCK_DGRAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr( server_ip );
	server_address.sin_port = htons(port);
	printf("Sending '%s' to %s:%i\n", message, server_ip, port);
	sendto( socket_id, message, strlen(message), 0,
		(struct sockaddr *) &server_address, sizeof(struct sockaddr_in) );
	printf("Waiting for Response\n");
	unsigned int return_len;
	recvfrom( socket_id, message, strlen(message), 0,
		(struct sockaddr *) &return_address, &return_len );
	printf("Received '%s'\n", message);
	close(socket_id);
}