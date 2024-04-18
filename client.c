#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdbool.h>


#define BUFFER_SIZE 1024
#define PORT_NUMBER 5437

void readInifile(const char* filename, char* ip_addr, int* port_num, int* timeout){
	FILE* file = fopen(filename, "r");
	if(file == NULL){
		printf("Failed to open the file.\n");
		return;
	}
	char line[100];
	while(fgets(line, sizeof(line), file)){

		if(strncmp(line, "ip =", 4) == 0){
			sscanf(line, "ip = %s", ip_addr);
		} else if(strncmp(line, "port =", 6) == 0){
			sscanf(line, "port = %d", port_num);
		} else if(strncmp(line, "timeout =", 9) == 0){
			sscanf(line, "timeout = %d", timeout);
		}
	}
	fclose(file);
}

int main(int argc, char *argv[])
{


	srand(time(NULL));
	int manual;
	int full = 0;


	int sockfd = 0, n = 0;
	char recvBuff[BUFFER_SIZE];
	struct sockaddr_in serv_addr;

	if(argc > 3 || argc == 1){
		printf("\n Usage: %s <manual>\n     or %s <automatic> <myinifile.txt>\n",argv[0],argv[0]);
		return 1;
	}
	printf("The cmd argument is %s \n", argv[1]);

	if(strcmp(argv[1],"manual") != 0 && strcmp(argv[1], "automatic") != 0){
		printf("\n Usage: %s <manual>\n     or %s <automatic> <myinifile.txt>\n",argv[0],argv[0]);
		return 1;
	}
	//needed initialization 
	int port_num;
	char ip_addr[16];
	int timeout;



	//manual choice
	if(strcmp(argv[1],"manual")==0){
		port_num = PORT_NUMBER;
		strcpy(ip_addr, "127.0.0.1");
		timeout = 100;
		manual = 1; 
	}
	//automatic choice

	if(strcmp(argv[1], "automatic") == 0 && strcmp(argv[2],"myinifile.txt")!=0 ){
		printf("\n Usage: %s <manual>\n     or %s <automatic> <myinifile.txt>\n",argv[0],argv[0]);
		return 1;
	}else if(strcmp(argv[1], "automatic") == 0 && strcmp(argv[2],"myinifile.txt")==0 ){
		readInifile("myinifile.txt", ip_addr, &port_num, &timeout);
		manual = 0;
	}

	//print check
	printf("IP: %s\n", ip_addr);
	printf("Port: %d\n", port_num);
	printf("Timeout: %d\n", timeout);


	while(full != 1){


		memset(recvBuff, '0',sizeof(recvBuff));
		if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
			printf("\n Error : Could not create socket \n");
			return 1;
		}

		memset(&serv_addr, '0', sizeof(serv_addr)); 

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(port_num); 

		if(inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr)<=0){
			printf("\n inet_pton error occured\n");
			return 1;
		}
		while(timeout > 0){
			if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
				if(timeout%5 == 0)printf("\n Error : Connect Failed \n Retrying to connect...\n");
				timeout--;
				sleep(1);
				if(timeout == 0){
					printf("Error : Connection timedout\n");
					return 1;
				}
			}else {
				break;
			}
		}
		int good, available;

		int r;
		int c;
		//while was here

		if(manual == 1){
			printf("Enter the row: ");
			scanf("%d", &r);
			printf("Enter the colomn: ");
			scanf("%d", &c);

			write(sockfd, &r, sizeof(r));
			write(sockfd, &c, sizeof(c));
			write(sockfd, &manual, sizeof(manual));

		}else if(manual == 0){
			r = rand();
			c = rand();
			int wait_time = ((rand()%3+1)*2)+1;
			sleep(wait_time);
			write(sockfd, &r, sizeof(r));
			write(sockfd, &c, sizeof(c));
			write(sockfd, &manual, sizeof(manual));

		}
		read(sockfd, &good, sizeof(good));
		read(sockfd, &available, sizeof(available));
		read(sockfd, &full, sizeof(full));

		if(good == 0) printf("The row/col is out of bound\n");
		if(available == 0 && full == 0) printf("The seat you requested is sold\n");
		if(full == 1) printf("All seats sold out!!\n");

		while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0){
			recvBuff[n] = 0;
			if(fputs(recvBuff, stdout) == EOF){
				printf("\n Error : Fputs error\n");
			}
		}

		if(n < 0){
			printf("\n Read error \n");
		}
	}//while ends here
	close(sockfd);
	return 0;
}
