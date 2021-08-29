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
     
      int sockfd;                 // return types of socket,bind and accept  
      struct sockaddr_in serv_addr,cli_addr;	 // server and client structure declaration of type sockaddr_in(man 7 ip for details) 
      int port_number;                  // server port number 
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

      //connect system call
      int retconnect=connect(sockfd,(struct sockaddr*)&serv_addr, sizeof(serv_addr));
      if(retconnect == 0)
              printf("Bind/connect successful with the server !\n");
      else
              printf("Connection failed with TCP server\n");		

       
      while(1)
      {  
       int num1,num2,sum;
      // clear the buffer
     // int buffer_count = 0;
      printf("\n=+=+=+=+=+=+Addition of Numbers=+=+=+=+=+=+\n");
      printf("Enter first number:");
      scanf("%d",&num1);
      printf("Enter second number:");
      scanf("%d",&num2);
   
      write(sockfd,&num1,sizeof(num1));
      write(sockfd,&num2,sizeof(num2));
      
      read(sockfd,&sum,sizeof(sum));
      printf("Message from server : %d\n",sum);   

      }
      close(sockfd);           // close the socket socket/file descriptor 
    
      return 0;

	
}	


