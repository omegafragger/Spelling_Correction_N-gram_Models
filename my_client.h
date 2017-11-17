#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

int connection()
{
	int sock = -1;
	struct sockaddr_in address;
	struct hostent * host;
	int len;


	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0)
	{
		printf("cannot create socket\n");
		return -1;
	}

	/* connect to server */
	address.sin_family = AF_INET;
	address.sin_port = htons(5001);
	host = gethostbyname("localhost");
	if (!host)
	{
		printf("error: unknown host\n");
		return -1;
	}
	memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
	if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
	{
		printf("error: cannot connect to host\n");
		return -1;
	}

	printf("connection created\n");

	return sock;
}

double extract_prob(char str[], double *prob, int sock, int number)
{
       char sendBuff[50];
	switch (number)
	{
	   case 1:
	       strcpy(sendBuff,"mode unigram");
	       break;
	   case 2:
	        strcpy(sendBuff,"mode bigram");
	        break;
	   case 3:
	        strcpy(sendBuff,"mode trigram");
	        break;
	   case 4:
	        strcpy(sendBuff,"mode quadragram");
	        break;
	   default:
	        printf("WRONG NUMBER\n");
	        break;
	 }

	 int len=strlen(sendBuff);
	 write(sock,sendBuff,len);

	char buffer[1024]="NULL";

        sleep(2);
	len=strlen(str);
	//write(sock,&len,sizeof(int));
	write(sock,str,len);

	read(sock,buffer,1024);

	double probability = atof(buffer);
        //printf("PROBABILITY RECEIVED !!! : %lf\n",probability);
	return probability;
}

void close_socket(int sock)
{

      close(sock);
}
