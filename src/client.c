#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main (int argc, char *argv[]) {

	if (argc != 3) {
		printf("USAGE: client <ip> <port>\n");
		return 1;
	}

	int socketfd;
	int portNumber = atoi(argv[2]);

	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddr.sin_port = htons(portNumber);

	socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(socketfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		perror("Connect error");
		return 1;
	}

	char buffer[1025];

	int n = recv(socketfd, buffer, 1024, 0);
	buffer[n] = '\0';

	printf("%s", buffer);

	char message[1025];

	while (strncmp(message, "exit", 4) != 0) {

		bzero(message, 1025);

		printf("Type your message:\n");
		fgets(message, sizeof(message), stdin);
		buffer[strcspn(buffer, "\n")] = 0;

		send(socketfd, message, strlen(message), 0);

		n = recv(socketfd, buffer, 1024, 0);
		buffer[n] = '\0';


		printf("Server: %s\n", buffer);

	}

	close(socketfd);



	return 0;
}