#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>

#define PI 4.0*atan(1.0)
#define BUFFER_SIZE 1024
#define PORT_NUMBER 5437

int count = 0;

void display(int* seats, int len, int wid){
	for(int i=0; i<len;i++){
		for(int j=0; j<wid; j++){
			printf("%d ",seats[i*wid+j]);
		}
		printf("\n");
	}

}
int main(int argc, char *argv[]){

	int len = 10, wid = 10;
	int r, c, manual;

	if(argc > 3 || argc == 2){
		printf("\n Usage: %s <length> <width> \n     or %s\n",argv[0], argv[0]);
		return 1;
	}
	if(argc == 3){
		len = atoi(argv[1]);
		wid = atoi(argv[2]);
		if(len == 0 && argv[1][0] != '0'){
			printf("\n Please enter an integer vlaue\n");
			return 1;
		}

		if(wid == 0 && argv[2][0] != '0'){
			printf("\n Please enter an integer vlaue\n");
			return 1;
		}
	}

	printf("\n here are the length %d and the width %d of the argument\n", len, wid);


	int size = len * wid;
	int seats[size];
	memset(seats,0,sizeof(seats));
	display(seats, len, wid);

	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	char sendBuff[BUFFER_SIZE];

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT_NUMBER);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);
	int full = 0;

	while(full != 1){
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

		read(connfd, &r, sizeof(r));
		read(connfd, &c, sizeof(c));

		read(connfd, &manual, sizeof(manual));
		int good = 1;
		int available = 1;

		if(manual==1){ // manual

			if(r>len || r<0 || c>wid || c<0 )good = 0;
			if(seats[r*wid+c] == 1)	available = 0;
			if(count == size) full = 1;

			printf("Client wants to buy a seat at row %d and colomn %d",r,c);

			if(good == 1 && available == 1 && full == 0){
				seats[r*wid+c] = 1;


				count++;

				printf(": Seat available \n");
				write(connfd, &good, sizeof(good));
				write(connfd, &available, sizeof(available));
				write(connfd, &full, sizeof(full));


			}else{

				printf(": Seat unavailable \n");
				write(connfd, &good, sizeof(good));
				write(connfd, &available, sizeof(available));
				write(connfd, &full, sizeof(full));
			}
		}else if(manual == 0){ // automatic
			r = r%len;
			c = c%wid;


			if(seats[r*wid+c] == 1)	available = 0;
			if(count == size) full = 1;

			printf("Client wants to buy a seat at row %d and colomn %d",r,c);

			if(available == 1 && full == 0){
				good = 1;
				seats[r*wid+c] = 1;
				count++;

				printf(": Seat available \n");
				write(connfd, &good, sizeof(good));
				write(connfd, &available, sizeof(available));
				write(connfd, &full, sizeof(full));

			}else{
				good = 1;


				printf(": Seat unavailable \n");
				write(connfd, &good, sizeof(good));
				write(connfd, &available, sizeof(available));
				write(connfd, &full, sizeof(full));

			}
		}


		display(seats, len, wid);


		time_t t = time(NULL);
		struct tm  * tm = localtime (&t);
		char s[64];
		strftime(s,sizeof(s),"%c",tm);
		if(good == 1 && available == 1 && full == 0){

			sprintf(sendBuff,"You bought a seat at row %d and colomn %d at %s\n",r,c,s);
		}else{
			sprintf(sendBuff,"Unable to buy a seat at row %d and colomn %d at %s\n",r,c,s);

		}
		write(connfd, sendBuff, strlen(sendBuff));

		close(connfd);
		sleep(1);
	}
	if(full == 1){
		printf("All seats sold out!!\n");
	}
}
