#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>


#define RECHISTRATION_FAIL -300 
#define SOCKET_COMM_ERROR  -301


int sock; 
struct sockaddr_in server;


int socket_init()
{
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
        return SOCKET_COMM_ERROR;
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");//local host
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
         //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("connect failed. Error");
        return SOCKET_COMM_ERROR;
    }
     
    puts("Connected\n");
    
return 0; 
  
}


int send_id_to_server(int message_code ) 
{

   char server_reply[256];
 
   uint32_t un = htonl(message_code);
   
         
        //Send id to server
        if( send(sock ,&un, sizeof(uint32_t), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
        if( recv(sock , server_reply , 256 , 0) < 0)
        {
            puts("recv failed");
            return 1;
        }
         
        puts("Server reply :");
        puts(server_reply);
        printf("\n");
        if(strcmp(server_reply , "ok") == 0)
        {
			printf(" cliaent  regestered \n");
	        return  0;
	    }
	    if (strcmp(server_reply , "register") == 0)
	        return 1;
	    if (strcmp(server_reply , "notregister") == 0)
	       return 2;   
	      	
    
     
    close(sock);
    return 0;
}


/*
char integer[4];                  // buffer
*((int*)integer) = 73232;         // 73232 is data which I want to send.
send( cs, integer, 4, 0 );
*/
