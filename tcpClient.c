#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{

	int clientSocket, ret,portno;
	struct sockaddr_in serverAddr;
	struct hostent *server;
	char buffer[1024];
	FILE *f;
	if (argc < 3)
    	{
       		fprintf(stderr,"usage %s hostname port\n", argv[0]);
       		exit(0);
    	}
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		error("Error in connection.\n");
	}
	printf("Client Socket is created.\n");
	portno = atoi(argv[2]);
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portno);
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		error("Error in connection.\n");
	}
	printf("Connected to Server.\n");
	f=fopen(argv[3],"r");
	char ch;
	write(clientSocket,argv[3],1024);
	while(ch !=EOF ){
		//printf("Client: \t");
		//scanf("%s", &buffer[0]);
		bzero(buffer,sizeof(buffer));
		fscanf(f , "%s" , buffer);
		printf("%s\n" , buffer);
		//send(clientSocket, buffer, strlen(buffer), 1024);
		//buffer="t";
		write(clientSocket,buffer,1024);
    		ch = fgetc(f);
    		printf("%c",ch);
	}
	fclose(f);
	close(clientSocket);
	return 0;
}
