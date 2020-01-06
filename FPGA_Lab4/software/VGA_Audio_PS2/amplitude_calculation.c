#include "amplitude_calculation.h"


void amplitude_operation(int size, int num, int rbuff_addr, int lbuff_addr, int dest_addr)
{
	amplitude_circute_stop();
	amplitude_circute_set_size(size);
	// also for your debugging make int amplitude_circute_get_size(); (optional)
	amplitude_circute_set_num(num);
	// also for your debugging make int amplitude_circute_get_num(); (optional)
	amplitude_circute_set_rbuff_addr(rbuff_addr);
	// also for your debugging make int amplitude_circute_get_lbuff_addr(); (optional)
	amplitude_circute_set_lbuff_addr(lbuff_addr);
	// also for your debugging make int amplitude_circute_get_rbuff_addr(); (optional)
	amplitude_circute_set_dest_addr(dest_addr);
	// also for your debugging make int amplitude_circute_get_dest_addr(); (optional)
	amplitude_circute_start();
	while(amplitude_circute_get_status() == 0);
	return;
}

int Base_Data = 0;
int Read_Data;
int Read_Reg0;
int Done_Bit;

void amplitude_circute_stop(){
	Read_Reg0 = IORD_ALT_UP_PARALLEL_PORT_DIRECTION(0x10004000);
	Done_Bit = Read_Reg0 >> 31;
	if(Done_Bit == 1){
		IOWR_ALT_UP_PARALLEL_PORT_DIRECTION(0x10004000,Base_Data || 1 << 31);
	}
	else if (Done_Bit == 0){
		IOWR_ALT_UP_PARALLEL_PORT_DIRECTION(0x10004000,Base_Data);
	}
}

void amplitude_circute_set_size(int size){
	Base_Data = size;
	Read_Reg0 = IORD_ALT_UP_PARALLEL_PORT_DIRECTION(0x10004000);
	IOWR_ALT_UP_PARALLEL_PORT_DIRECTION(0x10004000,Read_Reg0 || (Base_Data << 12));
}

void amplitude_circute_set_num(int num){
	Base_Data = num;
	Read_Reg0 = IORD_ALT_UP_PARALLEL_PORT_DIRECTION(0x10004000);
	IOWR_ALT_UP_PARALLEL_PORT_DIRECTION(0x10004000,Read_Reg0 || (Base_Data << 1));
}

void amplitude_circute_set_rbuff_addr(int Right_Address){
	IOWR_ALT_UP_PARALLEL_PORT_DIRECTION(0x10004004, Right_Address);
}

void amplitude_circute_set_lbuff_addr(int Left_Address){
	IOWR_ALT_UP_PARALLEL_PORT_DIRECTION(0x10004008, Left_Address);
}

void amplitude_circute_set_dest_addr(int Dest_Address){
	IOWR_ALT_UP_PARALLEL_PORT_DIRECTION(0x10004012, Dest_Address);
}

void amplitude_circute_start(){
	Base_Data = 1;
	IOWR_ALT_UP_PARALLEL_PORT_DIRECTION(0x10004000,Base_Data);
}

int amplitude_circute_get_status(){
	Read_Reg0 = IORD_ALT_UP_PARALLEL_PORT_DIRECTION(0x10004000);
	Done_Bit = Read_Reg0 >> 31;
	if(Done_Bit == 1){
		return 0;
	}
	if(Done_Bit == 0){
		return 1;
	}
	return 2;
}
