#include "globals.h"
#include "system.h"


void amplitude_operation(int size, int num, int rbuff_addr, int lbuff_addr, int dest_addr);

void amplitude_circute_stop();

void amplitude_circute_set_size(int size);

void amplitude_circute_set_num(int num);

void amplitude_circute_set_rbuff_addr(int Right_Address);

void amplitude_circute_set_lbuff_addr(int Left_Address);

void amplitude_circute_set_dest_addr(int Dest_Address);

void amplitude_circute_start();

int amplitude_circute_get_status();

int amplitude_circute_get_size();
