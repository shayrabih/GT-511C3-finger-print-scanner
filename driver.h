
#ifndef DRIVER_H_
#define DRIVER_H_



/*********command and response struct************/
typedef struct
 {
	char head1;
	char head2;
	short DevId;
	int nParam;
	short sCmd;
	short sChksum;
   
 }cmd_pck;
/************defines***********************************************/

#define UART1_CHAR_DEV "/dev/ttyUSB0"
#define STX1 0x55
#define STX2 0xAA
#define COMMAND_DEVICE_ID_1  0x01
#define COMMAND_DEVICE_ID_2  0x00
#define STRING_LENGTH_SIZE 12
#define CHECKSUMSIZE 10
#define FALSE 0
#define TRUE 1



/********command enum****************************************/

enum
{
	CMD_NONE				= 0x00,
	CMD_OPEN				= 0x01,
	CMD_CLOSE				= 0x02,
	CMD_USB_INTERNAL_CHECK	= 0x03,
	CMD_CHANGE_BAUDRATE		= 0x04,
	
	CMD_CMOS_LED			= 0x12,

	CMD_ENROLL_COUNT		= 0x20,
	CMD_CHECK_ENROLLED		= 0x21,
	CMD_ENROLL_START		= 0x22,
	CMD_ENROLL1				= 0x23,
	CMD_ENROLL2				= 0x24,
	CMD_ENROLL3				= 0x25,
	CMD_IS_PRESS_FINGER		= 0x26,
	
	CMD_DELETE				= 0x40,
	CMD_DELETE_ALL			= 0x41,
	
	CMD_VERIFY				= 0x50,
	CMD_IDENTIFY			= 0x51,
	CMD_VERIFY_TEMPLATE		= 0x52,
	CMD_IDENTIFY_TEMPLATE	= 0x53,
	
	CMD_CAPTURE				= 0x60,

	CMD_GET_IMAGE			= 0x62,
	CMD_GET_RAWIMAGE		= 0x63,
	
	CMD_GET_TEMPLATE	    = 0x70,
	
	CMD_ADD_TEMPLATE		= 0x71,
	CMD_GET_DATABASE_START  = 0x72,
	CMD_GET_DATABASE_END  	= 0x73,
	
	CMD_FW_UPDATE			= 0x80,
	CMD_ISO_UPDATE			= 0x81,
	
	ACK_OK					= 0x30,
	NACK_INFO				= 0x31,
};

/***************ERRORS ****************************/


enum 
{
    NO_ERROR =              0x1000,//ACTION SUCCESS
    NACK_TIMEOUT =          0x1001,// NACK_TIMEOUT 0x1001 Obsolete, capture timeout
    NACK_INVALID_BAUDRATE = 0x1002,// NACK_INVALID_BAUDRATE 0x1002 Obsolete, Invalid serial baud rate
    NACK_INVALID_POS =      0x1003,// NACK_INVALID_POS 0x1003 The specified ID is not between 0~1000
    NACK_IS_NOT_USED =      0x1004,// NACK_IS_NOT_USED 0x1004 The specified ID is not used
    NACK_IS_ALREADY_USED =  0x1005,// NACK_IS_ALREADY_USED 0x1005 The specified ID is already used
    NACK_COMM_ERR =         0x1006,// NACK_COMM_ERR 0x1006 Communication Error
    NACK_VERIFY_FAILED =    0x1007,// NACK_VERIFY_FAILED 0x1007 1:1 Verification Failure
    NACK_IDENTIFY_FAILED =  0x1008,// NACK_IDENTIFY_FAILED 0x1008 1:N Identification Failure
    NACK_DB_IS_FULL =       0x1009,// NACint ChangeBaudRate(int baud)K_DB_IS_FULL 0x1009 The database is full
    NACK_DB_IS_EMPTY =      0x100A,// NACK_DB_IS_EMPTY 0x100A The database is empty
    NACK_TURN_ERR =         0x100B, // NACK_TURN_ERR 0x100B Obsolete, Invalid order of the enrollment (The order was not as: EnrollStart -> Enroll1 -> Enroll2 -> Enroll3)
    NACK_BAD_FINGER =       0x100C, // NACK_BAD_FINGER 0x100C Too bad fingerprint
    NACK_ENROLL_FAILED =    0x100D,// NACK_ENROLL_FAILED 0x100D Enrollment Failure
    NACK_IS_NOT_SUPPORTED = 0x100E, // NACK_IS_NOT_SUPPORTED 0x100E The specified command is not supported
    NACK_DEV_ERR =          0x100F, // NACK_DEV_ERR 0x100F Device Error, especially if Crypto-Chip is trouble
    NACK_CAPTURE_CANCELED = 0x1010, // NACK_CAPTURE_CANCELED 0x1010 Obsolete, The capturing is canceled
    NACK_INVALID_PARAM =    0x1011,// NACK_INVALID_PARAM 0x1011 Invalid parameter
    NACK_FINGER_IS_NOT_PRESSED = 0x1012,// NACK_FINGER_IS_NOT_PRESSED 0x1012 Finger is not pressed,
    NACK_UNEXPECTED_ERROR = 0x1013 // NACK_UNEXPECTED ERROR general error happen

};

#define FPS_NONE	   -2000 //port not connected
#define FPS_COMM_ERR   -2001 /*serial error*/				
#define FPS_ERR_START	-500 
#define FPS_HDR_ERR		FPS_ERR_START+2
#define FPS_DEV_ID_ERR	FPS_ERR_START+3
#define FPS_CHK_SUM_ERR	FPS_ERR_START+4
#define FPS_PARAM_ERR	FPS_ERR_START+5
#define SOCKET_COMM_ERROR -301
#define REGISTRATION_REQUEST -302 


/*********function declaration***********************/
int init_dev_fps();
void uart_init(int fd);
void read_from_uart(int uart1_fd);
int write_to_uart(int uart1_fd, char* p ,int size);
short calcCmdChksum(cmd_pck* p);
int fps_SendCmdOrAck( short wDevID, short wCmdOrAck, int nParam );
int fps_CommandRun(short wCmd, int nCmdParam);
int fps_ReceiveCmdOrAck( short wDevID, short* pwCmdOrAck, int* pnParam );
int send_id_to_server(int message_code );
int socket_init();

/********fps commands implements  declarations ******************/

int enrollment();
int enroll_new_finger();
int identify_finger_print();
int fps_change_baudRate(int baudrate);
int fps_enroll_nth(int nTurn );
int fps_cmos_led( int bOn );
int fps_enroll_count( void );
int fps_check_enrolled( int nPos );
int fps_enroll_start( int nPos );
int fps_is_press_finger( void );
int fps_delete_all( void );
int fps_delete( int nPos );
int fps_identify( void );
int fps_verify( int nPos );
int fps_capture( int bBest );//enrollment 1 , id/verify 0.


#endif
