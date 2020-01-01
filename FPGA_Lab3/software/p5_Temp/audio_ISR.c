#include "globals.h"

/* globals used for audio record/playback */
extern volatile int buf_index_record, buf_index_play;
extern volatile unsigned int l_buf[BUF_SIZE];					// audio buffer
extern volatile unsigned int r_buf[BUF_SIZE];					// audio buffer
extern volatile unsigned int Echo_buf_r[BUF_SIZE];
extern volatile unsigned int Echo_buf_l[BUF_SIZE];
//extern volatile unsigned int Temp_l_buf[BUF_SIZE];					// audio buffer
//extern volatile unsigned int Temp_r_buf[BUF_SIZE];					// audio buffer
extern volatile unsigned int Echo_complete;
extern unsigned int Click_Echo;
extern unsigned int Echo_End;
extern unsigned int End_Record;

extern int Play_Index;
extern int Play_Flag;
extern int Clear_Play_Plot;

/***************************************************************************************
 * Audio - Interrupt Service Routine                                
 *                                                                          
 * This interrupt service routine records or plays back audio, depending on which type
 * interrupt (read or write) is pending in the audio device.
****************************************************************************************/
void audio_ISR(struct alt_up_dev *up_dev, unsigned int id)
{
	int num_read; int num_written;

	unsigned int fifospace;
		
	if (alt_up_audio_read_interrupt_pending(up_dev->audio_dev))	// check for read interrupt
	{
		alt_up_parallel_port_write_data (up_dev->green_LEDs_dev, 0x1); // set LEDG[0] on
		Echo_End = 0;

		// store data until the buffer is full
		if (buf_index_record < BUF_SIZE)
		{
			num_read = alt_up_audio_record_r (up_dev->audio_dev, &(r_buf[buf_index_record]), 
				BUF_SIZE - buf_index_record);
//			Temp_r_buf[buf_index_record] = r_buf[buf_index_record];
			/* assume we can read same # words from the left and right */
			(void) alt_up_audio_record_l (up_dev->audio_dev, &(l_buf[buf_index_record]), 
				num_read);
//			Temp_l_buf[buf_index_record] = l_buf[buf_index_record];
			buf_index_record += num_read;

			if (buf_index_record == BUF_SIZE)
			{
				// done recording
				End_Record = 1;
				alt_up_parallel_port_write_data (up_dev->green_LEDs_dev, 0); // turn off LEDG
				alt_up_audio_disable_read_interrupt(up_dev->audio_dev);
			}
		}
	}
	if (alt_up_audio_write_interrupt_pending(up_dev->audio_dev))	// check for write interrupt
	{

		// output data until the buffer is empty //Echo
		if (buf_index_play < BUF_SIZE && Echo_complete == 1 && Click_Echo == 1)
		{
			alt_up_parallel_port_write_data (up_dev->green_LEDs_dev, 0x4); // set LEDG[1] on
				num_written = alt_up_audio_play_r (up_dev->audio_dev, &(Echo_buf_r[buf_index_play]),
					BUF_SIZE - buf_index_play);
				/* assume that we can write the same # words to the left and right */
				(void) alt_up_audio_play_l (up_dev->audio_dev, &(Echo_buf_l[buf_index_play]),
					num_written);
				buf_index_play += num_written;

				if (buf_index_play >= BUF_SIZE)
				{
					// done playback
					alt_up_parallel_port_write_data (up_dev->green_LEDs_dev, 0); // turn off LEDG
					alt_up_audio_disable_write_interrupt(up_dev->audio_dev);
				}
		}

		// output data until the buffer is empty //Play
		else if (buf_index_play < BUF_SIZE && Click_Echo == 0)
		{

			alt_up_parallel_port_write_data (up_dev->green_LEDs_dev, 0x2); // set LEDG[2] on
			num_written = alt_up_audio_play_r (up_dev->audio_dev, &(r_buf[buf_index_play]), 
			 	BUF_SIZE - buf_index_play);
			/* assume that we can write the same # words to the left and right */
			(void) alt_up_audio_play_l (up_dev->audio_dev, &(l_buf[buf_index_play]), 
				num_written);
			buf_index_play += num_written;

			if(buf_index_play >= ((BUF_SIZE / N)*Play_Index)){
				Play_Flag = 1;
			}
	
			if (buf_index_play >= BUF_SIZE)
			{
				// done playback
				Play_Index = 0;
				alt_up_parallel_port_write_data (up_dev->green_LEDs_dev, 0); // turn off LEDG
				alt_up_audio_disable_write_interrupt(up_dev->audio_dev);
			}
		}
		else {
			alt_up_parallel_port_write_data (up_dev->green_LEDs_dev, 0x80); //  LEDG
			alt_up_audio_disable_write_interrupt(up_dev->audio_dev);
		}

	}
	return;
}
