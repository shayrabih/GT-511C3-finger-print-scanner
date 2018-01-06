#include "driver.h"
#include <stdlib.h>     /* General Utilities */
#include <termios.h>    /*For Uart conn*/
#include <stdio.h>
#include <fcntl.h>      
#include <unistd.h> 

/************global variable*************************/ 

volatile int STOPWRITE=FALSE;
volatile int STOPREAD=FALSE;   
const short gwDevID = 1; 
short gwLastAck = 0;
int  gwLastAckParam = 0;  
int uart1_fd;
int b0n = 0;
int bBest = 0;


int enroll_new_finger()
{
int enrolledId = 0 ,res = -1 , res1 = -1 ,ret;
volatile int usedId = TRUE;

printf("enroll new finger print ******************  \n\n");

while(usedId == TRUE)
  {
	fps_check_enrolled(enrolledId);
	if(enrolledId > 999 || enrolledId < 0)
			return NACK_INVALID_POS;
		
    else if(gwLastAckParam == NACK_IS_NOT_USED && enrolledId <= 999)
			usedId = FALSE	; //verify new id for new enrollment, continue ...
             		
	else			
	   enrolledId++;					
  }	
  
  printf("new id  : %d \n\n" , enrolledId);
  
  res = fps_enroll_start( enrolledId );
  
  //enroll1 : 
  printf("press finger for enroll 1  id#  %d \n\n" , enrolledId);
  ret = fps_CommandRun(CMD_CMOS_LED , 1);
  gwLastAckParam = -1;
   
   while(fps_is_press_finger() != 0) ;
   
   res = fps_capture(TRUE);

  if(gwLastAck == ACK_OK)
	{
	  printf("remove finger \n\n");
	  ret = fps_CommandRun(CMD_CMOS_LED , 0);
	  sleep(0.3);
	  res = fps_CommandRun(CMD_ENROLL1 , 0);
	  sleep(1);
	   while(fps_is_press_finger() == 0) ;//wait for remove finger
		
	  //enroll2
	  printf("press the same finger for enroll 2   \n\n" );
	  ret = fps_CommandRun(CMD_CMOS_LED , 1);
	  
	  while(fps_is_press_finger() != 0) ;//wait for press finger
	
	  res = fps_capture(TRUE);
	      if(gwLastAck == ACK_OK)
	        {
			  
			  res = fps_CommandRun(CMD_ENROLL2 , 0);	
			  sleep(1);
			  printf("remove finger \n\n");
			  ret = fps_CommandRun(CMD_CMOS_LED , 0);
			  sleep(0.3);
			  while(fps_is_press_finger() == 0) ;//wait for remove finger
				
			  //enroll3
			  printf("press the same finger for enroll 3   \n\n");
			   ret = fps_CommandRun(CMD_CMOS_LED , 1);  
			   	
			   while(fps_is_press_finger() != 0) ;//wait for press finger
			
			   res = fps_capture(TRUE);
					if( gwLastAck == ACK_OK)
						{
						  printf("remove finger \n\n");
						  ret = fps_CommandRun(CMD_CMOS_LED , 0);
						  sleep(0.3);
						  res1 = fps_CommandRun(CMD_ENROLL3 , 0);
						  sleep(0.3);	
						  if(gwLastAck == ACK_OK){
								printf("Enrolling Success!!!!!!! \n\n ");
								printf("new employee joined id  = %d ", enrolledId);
								return enrolledId;
							}
						  else
						       {
								 printf("enrolling failed with error code 0x%x \n\n" , gwLastAckParam);      
							   }	
							
						}
				else
					printf("failed to captured third finger\n\n");		
					   
			}
		else
		  printf("failed to captured second finger\n\n");
			
		
		}
		else
			printf("failed to captured first finger\n\n");
  
  
return NACK_FINGER_IS_NOT_PRESSED;	
}



int identify_finger_print()
{
	int res = -1, flag = 0  , ret = 0 ,id;
   
   
   printf("enroll  : please put your finger on device for 3 seconds \n" );
   fps_CommandRun(CMD_CMOS_LED , 1);
  
   
   while(flag == 0)
   {
	while (fps_is_press_finger() != 0);
	
	if(gwLastAckParam == 0)
	{
		fps_capture(0);
		 if(gwLastAck == ACK_OK)
					printf("finger print capture ... \n") ;
					
	   fps_identify();
	   if (gwLastAck == NACK_INFO && gwLastAckParam == NACK_DB_IS_EMPTY)
	         return  NACK_DB_IS_EMPTY;
	         
       else if(gwLastAck == ACK_OK && gwLastAckParam < 999 && gwLastAckParam >= 0)	
          {      
			
		  //identify success return new id;
			printf("identify ID : %d  \n" ,  gwLastAckParam);	
			id = gwLastAckParam;	
			res = fps_CommandRun(CMD_CMOS_LED , 0);
			flag  =	1;	//exit func 
			return id ;
		}
	    else
	       {
			//identify fail return not found
			printf("finger not found  , NACK_IDENTIFY_FAILED\n ");	
		    res = fps_CommandRun(CMD_CMOS_LED , 0);
			flag = 1; 
	        return NACK_IDENTIFY_FAILED;
	       } 
	 }   
	else
		printf("please press finger \n");    
	     
  }

	
return 0;	
	
}


int init_dev_fps()
{
	
	int ret;
    printf("*****start program******************** \n");
	uart1_fd = open(UART1_CHAR_DEV, O_RDWR /*| O_NOCTTY | O_NDELAY*/);  
    if(uart1_fd == -1) 
    {
        printf("ERROR Open uart1 /dev/ttySAC1 ! \n");
        exit(-1);
    }
    uart_init(uart1_fd);
    
    tcflush(uart1_fd,TCIOFLUSH);  
    sleep(0.3);		
	ret = fps_CommandRun(CMD_OPEN , 0);
	if (ret == 0)
		printf("fps device ready to use \n");
	else
	{
		printf("ERROR : open device fail \n");
		return NACK_COMM_ERR;
	}
	
	ret  = socket_init();
	if (ret == 0)
	   printf("socket created \n");
/*	else 
	   return  SOCKET_COMM_ERROR; 
	*/   
	   
	ret = fps_CommandRun(CMD_CMOS_LED , 1);	
	sleep(1);
	ret = fps_CommandRun(CMD_CMOS_LED , 0);
	sleep(1);	
	ret = fps_CommandRun(CMD_CMOS_LED , 1);
	sleep(1);
	ret = fps_CommandRun(CMD_CMOS_LED , 0);
	sleep(1);
	ret =  fps_enroll_count();
    printf("enroll count = %d \n " , gwLastAckParam );
    
 
	   
	   
return 0;	
}

int fps_capture( int bBest )
{
	return fps_CommandRun( CMD_CAPTURE, bBest );
}


int fps_verify( int nPos )
{
	return fps_CommandRun( CMD_VERIFY, nPos );
}

int fps_change_baudRate(int baudrate)
{
if (baudrate != 9600 || baudrate != 19200 || baudrate != 57600 || baudrate!= 38400 || baudrate != 115200)
    return 	NACK_INVALID_BAUDRATE;
 else
    return  fps_CommandRun( CMD_CHANGE_BAUDRATE, baudrate );
    	
	
return 0;	
}

int fps_identify( void )
{
return	 fps_CommandRun( CMD_IDENTIFY, 0 );
	 
}

int fps_delete_all( void )
{
	fps_CommandRun( CMD_DELETE_ALL, 0 );
	if(gwLastAck == ACK_OK)
		printf("all database deleted!!! \n\n");
return 0;		
}

int fps_delete( int nPos )
{
	return fps_CommandRun( CMD_DELETE, nPos );
}

int fps_is_press_finger( void )
{
	int res = -1;
    res =  fps_CommandRun( CMD_IS_PRESS_FINGER, 0 );
    if(res == 0)
    {
		if(gwLastAckParam == 0)
			return 0;	 
     }
 return 1;
}

int fps_enroll_count( void )
{
	return fps_CommandRun( CMD_ENROLL_COUNT, 0 );
	
}

int fps_check_enrolled( int nPos )
{
	 fps_CommandRun( CMD_CHECK_ENROLLED, nPos );
	 	if( gwLastAck == ACK_OK )
			return 0;
		
 return gwLastAckParam;
}

int fps_enroll_start( int nPos )
{
	return fps_CommandRun( CMD_ENROLL_START, nPos );
}


int fps_CommandRun(short wCmd, int nCmdParam)
{
	
	 
	if( fps_SendCmdOrAck( gwDevID, wCmd, nCmdParam ) < 0 )
	{
		printf("error from command run send cmd \n");
		return FPS_COMM_ERR;
		
	}
	
	if( fps_ReceiveCmdOrAck( gwDevID, &gwLastAck, &gwLastAckParam ) < 0 )
	{
		
	    printf("error from command run recieve cmd \n");
	    return FPS_COMM_ERR;
     }
return 0;
}


int fps_SendCmdOrAck( short wDevID, short wCmdOrAck, int nParam )
{
	cmd_pck pkt;
	int nSentBytes;
    
	pkt.head1 = (char)STX1;
	pkt.head2 = (char)STX2;
	pkt.DevId = wDevID;
	pkt.sCmd = wCmdOrAck;
	pkt.nParam = nParam;
	pkt.sChksum = calcCmdChksum( &pkt );
     printf("write data :   cmd : 0x%x         parameter : 0x%x " , wCmdOrAck , nParam  );
	nSentBytes = write_to_uart( uart1_fd , (char*)&pkt, STRING_LENGTH_SIZE );
	if( nSentBytes != STRING_LENGTH_SIZE )
	{
		printf("FPS_COMM_ERR on writing to USB \n");
		return FPS_COMM_ERR;
		
    }
	return 0;	
	

}



int write_to_uart(int uart1_fd , char* pBuf , int size)
{
	int ret ;	
	STOPWRITE = FALSE ; 
	
	 
	 printf("\n");
	  
     tcflush(uart1_fd,TCIOFLUSH);  
     sleep(0.3);
     
		while (STOPWRITE == FALSE)
	{		
		ret = write(uart1_fd,pBuf ,size);
	
		if(ret < 0)
		{
			printf("error while writing \n");
			sleep(0.2);
			STOPWRITE=FALSE;
		}
		
	  if(ret == STRING_LENGTH_SIZE)
			STOPWRITE=TRUE; 
	}	
	sleep(0.7);  
	
return ret;		
}


int fps_ReceiveCmdOrAck( short wDevID, short* pwCmdOrAck, int* pnParam )
{
  cmd_pck responce_pack;
  int nReceivedBytes ,ret ,i ,countReadBytes = 0;
  char firstbyte ,tmp;
  char *tmp1 = (char*)&responce_pack;
  STOPREAD = FALSE;
  
  	if( ( pwCmdOrAck == NULL ) || ( pnParam == NULL ) )				
				{
					printf("FPS_PARAM_ERR from reading \n");
					return FPS_PARAM_ERR;
					
				}
  
       
			
	while(STOPREAD == FALSE)
	{
	nReceivedBytes = read(uart1_fd,&firstbyte,1) ;	
		if(nReceivedBytes < 0)
			printf("Error reading ..... \n");
		if(firstbyte == STX1)
		{			
				tmp1[0] = firstbyte;
				countReadBytes++;				
					for(i=1;i<12;)
						{	
				    		ret = read(uart1_fd,&tmp,1);
							if(ret<0)
								sleep(0.1);
							else
							    {
								
								tmp1[i] = tmp;
								i++;	
								countReadBytes++;
					            }
	                    }
	          STOPREAD = TRUE;        
         }       		
   }
	
	if( countReadBytes != STRING_LENGTH_SIZE )
	{
		printf("FPS_COMM_ERR from reading \n");
		return FPS_COMM_ERR; 
	}	 
	if( ( responce_pack.head1 != STX1 ) || ( responce_pack.head2 != STX2 ) )
		
	{
		printf("FPS_HDR_ERR from reading \n");
		return FPS_HDR_ERR;
	}

	if( responce_pack.DevId != wDevID ) 
	{
		printf("FPS_DEV_ID_ERR from reading \n");
		return FPS_DEV_ID_ERR;
    }
	if( responce_pack.sChksum != calcCmdChksum( &responce_pack ) ) 
	{
		printf("FPS_CHK_SUM_ERR from reading \n");
	    return FPS_CHK_SUM_ERR;
	}
	*pwCmdOrAck = responce_pack.sCmd;
	*pnParam = responce_pack.nParam;	
	printf("reciev data :  wCmdOrAck :  0x%x   nParam: 0x%x   \n" , *pwCmdOrAck, *pnParam);
	
	
	printf("\n");	

return 0;	 
}





short calcCmdChksum(cmd_pck* p)
{
short CheckSum = 0;	
char* cmdtmp = (char*)p;
int i = 0;

for(;i<CHECKSUMSIZE;i++)
	CheckSum += cmdtmp[i];
 	
return CheckSum;	
	
	
}



void uart_init(int fd)
{
	struct termios options;

  	tcgetattr(fd, &options);  
  	
		cfsetispeed(&options, B9600);  
		cfsetospeed(&options, B9600);  
		options.c_cflag = (options.c_cflag & (~CSIZE)) | CS8;
        options.c_iflag &= IGNBRK;
        options.c_lflag = 0;
        options.c_oflag = 0;
  //      options.c_cflag |=  CREAD | CLOCAL;
        options.c_cc[VMIN] = 1;
        options.c_cc[VTIME] = 100;
        options.c_iflag &= ~(IXON|IXOFF|IXANY);
        options.c_cflag &= ~(PARENB | PARODD);


         /*options.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);*/
        /* Save The Configure */
        tcsetattr(fd, TCSANOW, &options);
        /* Flush the input (read) buffer */
        tcflush(fd,TCIOFLUSH);
}
