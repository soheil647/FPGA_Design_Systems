#include "globals.h"

extern volatile unsigned char byte1, byte2, byte3;
extern volatile unsigned char local_b1;
extern volatile unsigned char local_b2;
extern volatile unsigned char local_b3;
extern int count;
/***************************************************************************************
 * Pushbutton - Interrupt Service Routine                                
 *                                                                          
 * This routine checks which KEY has been pressed. If it is KEY1 or KEY2, it writes this 
 * value to the global variable key_pressed. If it is KEY3 then it loads the SW switch 
 * values and stores in the variable pattern
****************************************************************************************/

void PS2_ISR(struct alt_up_dev *up_dev, unsigned int id)
{

	unsigned char PS2_data;


	/* check for PS/2 data--display on HEX displays */
	if (alt_up_ps2_read_data_byte (up_dev->PS2_dev, &PS2_data) == 0)
	{
		count++;
		local_b1 = local_b2;
		local_b2 = local_b3;
		local_b3 = PS2_data;
		if(count == 3){
			byte1 = local_b1;
			byte2 = local_b2;
			byte3 = local_b3;
			count = 0;
		}
		/* allows save the last three bytes of data */
//		byte1 = byte2;
//		byte2 = byte3;
//		byte3 = PS2_data;
		if ( (local_b2 == (unsigned char) 0xAA) && (local_b3 == (unsigned char) 0x00) )
			// mouse inserted; initialize sending of data
			(void) alt_up_ps2_write_data_byte (up_dev->PS2_dev, (unsigned char) 0xF4);
	}

	return;
}
