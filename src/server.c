#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main (int argc, char *argv[]) {

	int listenfd, clientfd;
	struct sockaddr_in serverAddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		perror("Socket error");
		return 1;
	}
	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(6160);

	int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		perror("Bind error");
		return 1;
	}

	if (listen(listenfd, 10) < 0) {
		perror("Listen error");
		return 1;
	}

	clientfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
	if (clientfd < 0) {
		perror("Accept error");
		return 1;
	}
	printf("Received a connection in %d\n", clientfd);

	char buffer[1024];

	char sendBuffer[] = "Welcome to the server.\nType exit to exit.\n";


	if (send(clientfd, sendBuffer, strlen(sendBuffer), 0) < 0) {
		perror("Send error");
		return 1;
	} 

	while (strncmp(buffer, "exit", 4) != 0) {

		bzero(buffer, 1024);

		int n = recv(clientfd, buffer, 1024, 0);
		if (n <= 0) {
			printf("Client disconnected or an error has occurred.\n");
			break;
		}
		buffer[n] = '\0';
		buffer[strcspn(buffer, "\n")] = 0;

		printf("Sending back: %s\n", buffer);
		if (send(clientfd, buffer, strlen(buffer), 0) < 0) {
			perror("Send error");
			break;
		}

	}

	close(clientfd);
	close(listenfd);
	
	return 0;
}