
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <sys/wait.h>   /* Wait for Process Termination */
#include <stdlib.h>     /* General Utilities */
#include <termios.h>    //for uart comm  
#include <fcntl.h>
#include <string.h>
#include "driver.h"
#include <time.h>    //for time
#include <stdlib.h> //for file 


 struct employee
 {
	 char Employeename[30];
	 int Entry;
	 int id;
 };





int main()
{
	struct tm *timeinfo;
	char str_to_print_log[60] = {0};	 
    struct employee arr[10];
	int fd_log;
    time_t result;
    time_t now = time(0);
    int retVal = -1 ,ret ; 
    char name[25];
    char time_buff[20];
    int size; 
    char newline[] = "\n";
    
    
    fd_log = open("/home/plg/log.txt", O_RDWR | O_APPEND | O_NOCTTY | O_NDELAY);
    if(fd_log == NULL)
	    printf("ERROR open  /home/plg/log.txt"); 
	
	
	timeinfo = localtime(&result);     
    retVal =  init_dev_fps();
   
   if(retVal == 0)
		printf("device init success\n");
printf("**************** AFTER INIT ************************************ \n");
 // retVal = fps_delete_all();
while(1)
{
 
 // while(fps_is_press_finger() != 0) ;
		

    retVal = -1;
    
   ret =  identify_finger_print();
   if(ret != NACK_IDENTIFY_FAILED && ret != NACK_DB_IS_EMPTY)
       {	//if success identify return verify ID
		   time(&result);
		   timeinfo = localtime(&result);
		   strftime (time_buff, sizeof(time_buff), "%Y-%m-%d %H:%M:%S ",timeinfo);
		   strcpy(str_to_print_log,arr[ret].Employeename );
		   strcat(str_to_print_log, arr[ret].Entry==1?"exit  ":"enter  ");
		   strcat(str_to_print_log,time_buff);
		   printf("name : %s  id : %d  entrance : %d  %s \n", arr[ret].Employeename  ,arr[ret].id , arr[ret].Entry,str_to_print_log);
		   size = write(fd_log,str_to_print_log,sizeof(str_to_print_log));
		   printf("write to log file %d \n" , size);	
		   size = write(fd_log,newline,sizeof(newline));   
		   arr[ret].Entry= !(arr[ret].Entry);
	       memset(str_to_print_log,0,strlen(str_to_print_log));	
	}
  
     else  if (ret == NACK_IDENTIFY_FAILED || ret ==  NACK_DB_IS_EMPTY )//client will register  in DB
	 {
	   
	   retVal = enroll_new_finger();
			if(retVal >= 0 && retVal < 1000)
	               { //if success retVal get the new id that created
						printf("enter name first name:  ");
						scanf("%s" ,name);
						printf("\n");
						strcpy(arr[retVal].Employeename,name);
						printf("enter name last name:  ");
						scanf("%s" ,name);
						printf("\n");
						strcpy(arr[retVal].Employeename,name);
						arr[retVal].Entry = 0;
						arr[retVal].id = retVal;
						printf("new employee name : %s id = %d  \n" , arr[retVal].Employeename , arr[retVal].id);		
			       }
	else
	  printf("invalid id from new enrollment!! \n")	;
     }

    fps_CommandRun(CMD_CMOS_LED,0);     	
	printf("end program....... go again!!!!!\n");
	sleep(2);
}	
  return 0;
	
}

