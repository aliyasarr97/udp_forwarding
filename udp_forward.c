#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFLEN 1024
#define PORT   2152
#define PACKAGE_COUNTER_DELAY 5

int count = 0;

void sys_exit(char *s)
{
	perror(s);
	exit(1);
}

/*This thread shows incoming package count
  on every one minute*/
void *package_count(void *data)
{
    while(1)
    {
        sleep(PACKAGE_COUNTER_DELAY);
        printf("Total package count = %d \n", count);

    }
    return NULL;
}

int main(void) {
    
    pthread_t package_count_thread;
    struct sockaddr_in server, client;
	
	int fdClient, recv_len;
    int slen = sizeof(server);
	char buf[BUFLEN];
	
    if ((fdClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		sys_exit("socket");
	}
 
	memset((char *) &client, 0, sizeof(client));
	
	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	client.sin_addr.s_addr = htonl(INADDR_ANY);
	
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

	if( bind(fdClient , (struct sockaddr*)&client, sizeof(client) ) == -1)
	{
		sys_exit("bind error\n");
	}

    pthread_create(&package_count_thread, NULL, package_count, NULL);
    
    while(1)
    {
        //printf("Data waiting\n");
        if ((recv_len = recvfrom(fdClient, buf, BUFLEN, 0, (struct sockaddr *) &client, &slen)) == -1)
		{
			sys_exit("Message could not received!");
		}

        //printf("Received packet from %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));
        server.sin_addr = client.sin_addr;

        if (sendto(fdClient, buf, recv_len, 0, (struct sockaddr*) &server, slen) == -1)
		{
			printf("Message could not send!\n");
		}
        ++count;
        //printf("Message forwarded to %s:%d\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));
    }

    printf("Exiting from server program\n");
    return 0;
}