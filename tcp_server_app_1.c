/*
Author: Saurabh Mane
Topic : Connect the client and receive the message and disconnects after sending

*/

#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>               // header for socket system call
#include<stdlib.h>
#include<netinet/in.h>               //man 7 ip for (socket address) 
#include<netinet/ip.h>
#include<string.h> 
#include<arpa/inet.h>               // hton man 3 tons

/******Handler for errors***********/

void error(char *msg)
{
     perror(msg);
     exit(1);
}

int main(int argc,char *argv[])
{	
     
      int sockfd,retbind,connfd;                 // return types of socket,bind and accept  
      struct sockaddr_in serv_addr,cli_addr;	 // server and client structure declaration of type sockaddr_in(man 7 ip for details) 
      int port_number, cli_len;                  // server port number and client length variables
      char buffer[1024];                         // buffer to be used for reading and writing

      //clear the buffer of the structure
      bzero((char*)&serv_addr,sizeof(serv_addr));
     
      //condition check for command line argument 
      if(argc < 2)
      {
            printf("Kindly provide the port number !!\n");
            printf("Use this format : ./filename <portnumber>\n");
      }

      // first argument on command line is port number 
      port_number=atoi(argv[1]);             //typecast to interger format
      
      //initialize the structure members
      
      /*
       Remember !!
       IP address and port number  must be in the network byte order which is "Big endian"
       So keep uniformity between client and server endianness it is good practice to
       explicitly make it to the network byte order
       */

      serv_addr.sin_family=AF_INET;        // address family->IPv4
      serv_addr.sin_port=htons(port_number); //host byte order to network byte order(short int)
      serv_addr.sin_addr.s_addr=INADDR_ANY;  // can be any IP address like "your system IP/loopback/localhost"



      //create a listening socket to accept incoming request(used only fot listening)
      sockfd=socket(AF_INET,SOCK_STREAM,0);  //TCP-SOCK_STREAM

      // handle the error if socket is not created	      
      if(sockfd<0)
      {
	      	error("Socket creation failed !!!\n");
	      exit(1);
      }	   
      else
      {
              printf("Master socket creation successful !\n");
      }

      retbind=bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    
      if(retbind<0)
      {
           error("Failed to bind the IPV4 address to socket !!\n");
           exit(1);
      }
      else
      {
           printf("Server is running at port number : %d\n",port_number);
      }

      int number_of_backlog_connections=5;
      
      int retlisten=listen(sockfd,number_of_backlog_connections);

      if(retlisten == 0)
      {
	      printf("Listen system call is working successfully\n");
      }	      
      else
      {
	      printf("Listen system call failed to keep connections in the backlog\n");
      }

      cli_len=sizeof(cli_addr);

      //accept system call
      connfd = accept(sockfd,(struct sockaddr*)&cli_addr, &cli_len);
      
      if(connfd < 0)
      {
	     printf("Unable to create socket using accept system call\n"); 
	     exit(0);
      }
      else
      {
	      printf("connfd socket descriptor is created successfully having number as %d\n",connfd);
      }


      while(1)
      {
      int num1,num2;
      read(connfd,&num1,sizeof(num1));
      read(connfd,&num2,sizeof(num2));
      int sum=num1+num2;
      printf("Message from client : %d\n",sum);
       // clear the buffer
      write(connfd,&sum,sizeof(sum));
     
      
      }
      close(connfd);           // close the accept socket/file descriptor
      close(sockfd);           // close the socket socket/file descriptor 
    
      return 0;

	
}	


