#include "globals.h"

/* global variables */
volatile int buf_index_record, buf_index_play;		// audio variables

volatile unsigned char byte1, byte2, byte3;			// PS/2 variables
volatile unsigned char local_b1,local_b2,local_b3;
volatile unsigned int l_buf[BUF_SIZE];					// audio buffer
volatile unsigned int r_buf[BUF_SIZE];					// audio buffer
//volatile unsigned int Temp_l_buf[BUF_SIZE];					// audio buffer
//volatile unsigned int Temp_r_buf[BUF_SIZE];					// audio buffer
volatile unsigned int Echo_buf_r[BUF_SIZE];
volatile unsigned int Echo_buf_l[BUF_SIZE];
volatile unsigned int Echo_complete;
unsigned int Echo_End;
unsigned int End_Record;
unsigned int Click_Echo;
int count;
int Play_Index;
int Play_Flag;
int Clear_Play_Plot;

volatile int timeout;										// used to synchronize with the timer

struct alt_up_dev up_dev;									/* struct to hold pointers to open devices */
