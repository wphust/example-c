/*
 ============================================================================
 Name        : UDPClient.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//#include <stdio.h>
//#include <stdlib.h>
//
//int main(void) {
//	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
//	return EXIT_SUCCESS;
//}



/*
 ============================================================================
 Name        : UDPClient.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*
    Simple udp client
*/
#include<stdio.h>   //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define SERVER "10.6.4.34"
#define BUFLEN 512  //Max length of buffer
#define PORT 4567   //The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_other;
    int s, i;
    int slen = sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    while(1)
    {
        printf("Enter message : ");
        gets(message);

        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }

        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        int recv_len;
        recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
        if (recv_len == -1)
        {
            die("recvfrom()");
        } else
            printf("received data:%d\n",recv_len);

        puts(buf);
    }

    close(s);
    return 0;
}
