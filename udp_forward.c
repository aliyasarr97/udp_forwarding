#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int count = 0;

/*This thread shows incoming package count
  on every one minute*/
void *package_count(void *data)
{
    while(1)
    {
        count++;
        sleep(1);
        printf("count=%d \n", count);

    }
    return NULL;
}

int main(void) {
    pthread_t th1;
    pthread_create(&th1, NULL, package_count, NULL);
    
    while(1)
        ;

    printf("Exiting from main program\n");
    return 0;
}