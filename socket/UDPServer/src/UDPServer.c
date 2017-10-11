/*
 ============================================================================
 Name        : UDPServer.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*
 ============================================================================
 Name        : UDPServer.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//#define MULTI_SELECT

#ifdef MULTI_SELECT
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
//#include "slist.c"
#define MAXLEN 4096

char* toUpper(char*);

int main(int argc, char **argv)
{
    //slist_t *list = (slist_t*)malloc(sizeof(slist_t));
    //slist_init(list);
    fd_set read_fds, write_fds;             // temp file descriptors list for select()
    int sockfd;                             // socket descriptor
    struct sockaddr_in srv, cli_addr;       // srv used by bind()
    socklen_t clilen = sizeof(cli_addr);
    int nbytes, portno;

    //check command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: ./server %s <port>\n", argv[1]);
        exit(1);
    }
    portno = atoi(argv[1]);
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        perror("ERROR opening socket");
        exit(1);
    }
    else
        printf("Server : Socket() successful: %d \n",portno);

  /* bind: use the Internet address family: */
    /* create the socket */
    srv.sin_family = AF_INET;
    /* bind: socket ‘sockfd’ to port portno*/
    srv.sin_port = htons(portno);
    /* bind: a client may connect to any of my addresses */
    srv.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr*) &srv, sizeof(srv)) < 0){
        perror("ERROR on binding");
        close(sockfd);
        exit(1);
    }
    else
        printf("Server : bind() successful\n");
    //Now the UDP server is ready to accept packets…
    while(1){
        char* buf = (char*)malloc(MAXLEN);              // message buf
        char* copybuf = (char*)malloc(MAXLEN);
        int j;
        memset(buf, 0, strlen(buf));
        bzero(buf, MAXLEN);
        FD_ZERO(&read_fds);
        FD_SET(sockfd, &read_fds);
        int n = select(sockfd+1, &read_fds, &write_fds, 0, 0);
        if(n < 0)
        {
            perror("ERROR Server : select()\n");
            close(sockfd);
            exit(1);
        }
        if(FD_ISSET(sockfd, &read_fds)) {
            printf("Server is ready to read\n");
            nbytes = recvfrom(sockfd, buf, MAXLEN, 0, (struct sockaddr*)&cli_addr, &clilen);
            if (nbytes < 0){
                perror("ERROR in recvfrom()");
                close(sockfd);
                exit(1);
            }
            j = ntohs(cli_addr.sin_port);
            FD_CLR(sockfd, &read_fds);
        }
        if(strlen(buf) != 0){
            memcpy(copybuf, buf, strlen(buf));
            int i = strlen(buf) + 3 + strlen(inet_ntoa(cli_addr.sin_addr)) + ntohs(cli_addr.sin_port);
            char* c = (char*) malloc(i);
            char ch[5];
            strcpy(c, inet_ntoa(cli_addr.sin_addr));
            strcat(c, " : ");
            sprintf(ch, "%d", j);
            strcat(c, ch);
            strcat(c, " : ");
            strcat(c, buf);
            //slist_append(list, c);
            //printlist(list);              //you may see the list
//            if(list == NULL){
//                perror("List is empty. Server is not ready to write.");
//                close(sockfd);
//                exit(1);
//            }
//            else
            {
                FD_ZERO(&write_fds);
                FD_SET(sockfd, &write_fds);
                if(FD_ISSET(sockfd, &write_fds)){
                    toUpper(copybuf);
                    nbytes = sendto(sockfd, copybuf, MAXLEN, 0, (struct sockaddr*) &cli_addr, sizeof(cli_addr));
                    if(nbytes < 0) {
                        perror("ERROR in sendto");
                        close(sockfd);
                        exit(1);
                    }
                    printf("Server is ready to write\n");
                    //printf("Send to client: %s", copybuf);
                    //printf("==========================\n");
                    printf("\n");
                    memset(&copybuf, 0, sizeof(copybuf));
                    FD_CLR(sockfd, &write_fds);
                }
            }
            bzero(ch, strlen(ch));
        }
        memset(&buf, 0, sizeof(buf));
        free(buf);
        free(copybuf);
    }
//    free(list);
    close(sockfd);
    return 0;
}
char* toUpper(char* str){
    if(str != NULL){
        int i;
        for(i = 0; i<strlen(str); i++)
        {
            if( str[i] <= 'z' && str[i] >= 'a')
            {
                str[i] += 'A' - 'a';
            }
        }
        return str;
    }
    return NULL;
}

#else
/*
    Simple udp server
*/
#include<stdio.h>   //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>


#define BUFLEN 512  //Max length of buffer
#define PORT 4567   //The port on which to listen for incoming data

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_me, si_other;

    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }

    printf("Open UDP socket %s:%d\n", inet_ntoa(si_me.sin_addr), ntohs(si_me.sin_port));

    //keep listening for data
    while(1)
    {
        printf("Waiting for data22...");
        fflush(stdout);

        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }

        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);

        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }

    close(s);
    return 0;
}
#endif
