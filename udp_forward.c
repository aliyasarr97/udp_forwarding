#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFLEN 512
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
    struct sockaddr_in rcv;
	
	int s, i, recv_len;
    int slen = sizeof(rcv);
	char buf[BUFLEN];
	
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		sys_exit("socket");
	}
	 
	memset((char *) &rcv, 0, sizeof(rcv));
	
	rcv.sin_family = AF_INET;
	rcv.sin_port = htons(PORT);
	rcv.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if( bind(s , (struct sockaddr*)&rcv, sizeof(rcv) ) == -1)
	{
		sys_exit("bind error\n");
	}

    pthread_create(&package_count_thread, NULL, package_count, NULL);
    
    while(1)
    {
        printf("Data waiting\n");

        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &rcv, &slen)) == -1)
		{
			sys_exit("Message could not received!");
		}

        count++;
        printf("Received packet from %s:%d\n", inet_ntoa(rcv.sin_addr), ntohs(rcv.sin_port));
		printf("Data: %s\n" , buf);
    }

    printf("Exiting from server program\n");
    return 0;
}