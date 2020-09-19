#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){

	int sockfd, ret,portno;
	 struct sockaddr_in serverAddr;

	int newSocket,i,k=0,flag=0;
	struct sockaddr_in newAddr;

	socklen_t addr_size;
	char dir[100];
	char buffer[1024];
	pid_t childpid;
	for(i=1;argv[2][i]!='\0';i++)
		dir[k++]=argv[2][i];
	dir[k]='\0';
	printf("%s",dir);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("Error in connection.");
	}
	printf("Server Socket is created.\n");
	portno = atoi(argv[1]);
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portno);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		error("Error in binding.");
		exit(1);
	}
	printf("Bind to port %d\n", portno);

	if(listen(sockfd, 10) == 0){ // possible to have 10 clients
		printf("Listening....\n");
	}else{
		printf("Error in binding.\n");
	}

	 FILE *fp;
         int ch = 0,len;
	if(mkdir(dir,0777)==-1)
		printf("Directory already exist\n");
	else
		printf("Director created\n");
	getcwd(dir, sizeof(dir));
	strcat(dir,argv[2]);
	//strcat(dir,"/glad_received.txt");
        //fp = fopen(dir,"w");
	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		// create multiple clients.. max of 10
		if((childpid = fork()) == 0){
			
			close(sockfd);

			while(1){
				len=recv(newSocket, buffer, 1024, 0);
				if(len==0){
					fclose(fp);
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Client: %s\n", buffer);
					
					if(flag==0)
					{
						strcat(dir,"/rec");
						strcat(dir,buffer);
						printf("%s",dir);
						fp = fopen(dir,"w"); // receiving seperate for each client
						flag=1;
					}
					else
						fprintf(fp , " %s" , buffer);
					//send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
			
		}
	}

	close(newSocket);


	return 0;
}
