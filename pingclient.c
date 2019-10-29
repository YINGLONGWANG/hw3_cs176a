#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
char * get_message();


int main(int argc, char * argv[]) {
	struct sockaddr_in server_address, return_address;
	
	char server_ip[] = "127.0.0.1";
	int port = 12000;
	
	const char * message = get_message();
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

char * get_message(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long double millisecondsSinceEpoch =
    		(long double)(tv.tv_sec) * 1000 +
    		(long double)(tv.tv_usec) / 1000;
	char* time = malloc(100);
	sprintf(time, "%Lf", millisecondsSinceEpoch);

	char * message= malloc(400);
	strcpy(message, "PING ");
	strcat(message, time);
	

	return message;
}
