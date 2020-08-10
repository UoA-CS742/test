// gcc server.c -o server
// indent -npro -kr -i8 -ts8 -sob -l280 -ss -ncs -cp1 *

/***************************************************************************/
/* */
/* Server program which wait for the client to connect and reads the data */
/* using non-blocking socket. */
/* The reading of non-blocking sockets is done in a loop until data */
/* arrives to the sockfd. */
/* */
/* based on Beej's program - look in the simple TCP server for further doc.*/
/* */
/* */
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>        /* Added for the nonblocking socket */
#include <unistd.h>
#include <sys/select.h>
#include <iostream>

#define MYPORT 3456        /* the port users will be connecting to */
#define BACKLOG 10        /* how many pending connections queue will hold */
#define  IP_ADDR      "0.0.0.0" //local IP address
int main()
{
    int sockfd, new_fd;    /* listen on sock_fd, new connection on new_fd */
    struct sockaddr_in my_addr;    /* my address information */
    struct sockaddr_in their_addr;    /* connector's address information */
    socklen_t sin_size;
    char string_read[255];
    int n, i;
    int last_fd;        /* Thelast sockfd that is connected */
    socklen_t len;
    len=sizeof(their_addr);
    const char *hello = "Because i am the BEST YASUO OCE";

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    last_fd = sockfd;

    my_addr.sin_family = AF_INET;    /* host byte order */
    my_addr.sin_port = htons(MYPORT);    /* short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY;    /* auto-fill with my IP */
    bzero(&(my_addr.sin_zero), 8);    /* zero the rest of the struct */

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }


    fcntl(sockfd, F_SETFL, O_NONBLOCK);    /* Change the socket into non-blocking state */
    //fcntl(new_fd, F_SETFL, O_NONBLOCK);    /* Change the socket into non-blocking state */

    int selectReturn;
    fd_set rfd;
    timeval timeout= {3,0};

    while(true){
    	FD_ZERO(&rfd);
    	FD_SET(sockfd, &rfd);
    	selectReturn = select(sockfd+1, &rfd, NULL, NULL, &timeout);
    	if(selectReturn == -1){
    		printf("select()/n");
    		close(sockfd);
    		exit(-1);

    	}else{

    		if(FD_ISSET(sockfd, &rfd)){
    			int nbytes = recvfrom(sockfd,string_read,sizeof(string_read),0,(struct sockaddr*)&their_addr,&len);
    			if (nbytes == -1){
    				perror("Recvfrom failed");
    				return -1;
    			}
    			if(nbytes>0){
    				string_read[nbytes]=0;
    				printf("The message received from %s  Data is :%s\n",inet_ntoa(their_addr.sin_addr),string_read);
    			}
    			sendto(sockfd,(const char *)hello,strlen(hello),MSG_CONFIRM,(const struct sockaddr*)&their_addr,len);
    		}
    	}
    }
    return 0;
}
