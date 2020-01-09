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
	while(amplitude_circute_get_status() == busy);
	return;
}
