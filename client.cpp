// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <iostream>
#include <chrono>
#include <netinet/tcp.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define PORT     3456 //change the port if you want
#define MAXLINE 1024
#define IP_ADDR "0.0.0.0" //please change the IP address, change to 127.0.0.1 for local testing e.g. virtualbox or vmware
using namespace std::chrono;
using namespace std;

// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    const char *hello = "Why you pick Yasuo on my BO5?";
    char *data;
    int packetsize = 10000000; //10MB
    struct sockaddr_in  servaddr;
    int r;
    int s;
    int *optval;
    int totalbyte = 0;
    double Mbs = 0;
    double totaltime = 0;
    const size_t defaultSendLength = 1024 * 10 * 50;
    char buff[defaultSendLength];
    //char buff[2000];

    socklen_t optlen;
    socklen_t len;
    len = sizeof(servaddr);
    struct tcp_info info;
    optlen = sizeof(info);

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(IP_ADDR);
   // struct timeval time1; 
   // struct timeval time2;
    struct timespec starttime, finishtime; //setup the start time and finish time for RTT
   // gettimeofday(&time1, NULL);            
   // double t1 = time1.tv_sec;
   // std::cout << "t2 and t1: " <<t1<<"\n";
    //double t1 = time1.tv_usec;
    clock_gettime(CLOCK_REALTIME, &starttime);  // get the start time

    int i = 0;
    int x = 0;
    while(i<1000){ 
    cout << "byte: " << servaddr.sin_addr.s_addr << "\n";  //print how many bytes have sent to the server
    
    s = sendto(sockfd, &hello, 9000 ,MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr)); // bind to all the information to the socket and send to the server
    //s = sendto(sockfd, buff, 1024,MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
    cout << "byte: " << s << "\n";  //print how many bytes have sent to the server
    if(s < 0 ){
    	perror("Sendto error: ");
    	exit(1);
    } else{
    	totalbyte += s; //caluclate how many bytes have been sent to the server

    }

    r = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr,&len); //receive reply message from the server
    if (r < 0){
    	perror("Recvfrom error: ");
    	exit(1);
    }
    buffer[r] = '\0';


    std::cout << "Reply from: " << inet_ntoa(servaddr.sin_addr) << " " << "Port: " << ntohs(servaddr.sin_port) << " " << "Message: " << buffer << " " << "\n"; //print out the receive message

    i++;


    }

    close(sockfd);
    clock_gettime(CLOCK_REALTIME, &finishtime); //setup the finish time
    //gettimeofday(&time2, NULL);

   // double t2 = time2.tv_sec;
    cout << "finishtime: " << finishtime.tv_nsec << "\n"; //print out the finsh time in nanosec
    cout << "starttime: " << starttime.tv_nsec << "\n";   //print out the start time in nanosec
    cout << "Totalbyte: " << totalbyte << "\n";           //print out the total bytes have sent to the server
    //cout << "t2 and t1: " << t2 <<" "<<t1<<"\n";
    double t3 =  (finishtime.tv_nsec -  starttime.tv_nsec) * 1e-9;   //find out the time elapsed and convert to second
    cout << "gettimeofday: " << t3<<"\n";                       //print the elapsed time
    Mbs = (totalbyte*1e-6)/t3;                           //total bytes convert to megabytes then divide by elapsed time = Mbs
    cout << "Throughput: " << Mbs << "Mbs"<< "\n";       
    return 0;

}
