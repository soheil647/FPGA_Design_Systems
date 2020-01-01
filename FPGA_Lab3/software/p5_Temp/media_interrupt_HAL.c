#include "globals.h"
#include "math.h"
#include "stdlib.h"


alt_16 coeff[64] =
{
		0xff7d                                                        ,
		0xfeb5                                                        ,
		0xfdde                                                        ,
		0xfd4d                                                        ,
		0xfd9f                                                        ,
		0xfecf                                                        ,
		0x008b                                                        ,
		0x01d9                                                        ,
		0x0200                                                        ,
		0x00b5                                                        ,
		0xfec2                                                        ,
		0xfd63                                                        ,
		0xfdbe                                                        ,
		0xffd3                                                        ,
		0x0275                                                        ,
		0x03bf                                                        ,
		0x027e                                                        ,
		0xff13                                                        ,
		0xfba3                                                        ,
		0xfabd                                                        ,
		0xfdae                                                        ,
		0x0320                                                        ,
		0x0797                                                        ,
		0x075a                                                        ,
		0x0134                                                        ,
		0xf7f0                                                        ,
		0xf1a2                                                        ,
		0xf493                                                        ,
		0x0386                                                        ,
		0x1b62                                                        ,
		0x33ef                                                        ,
		0x4375                                                        ,
		0x4375                                                        ,
		0x33ef                                                        ,
		0x1b62                                                        ,
		0x0386                                                        ,
		0xf493                                                        ,
		0xf1a2                                                        ,
		0xf7f0                                                        ,
		0x0134                                                        ,
		0x075a                                                        ,
		0x0797                                                        ,
		0x0320                                                        ,
		0xfdae                                                        ,
		0xfabd                                                        ,
		0xfba3                                                        ,
		0xff13                                                        ,
		0x027e                                                        ,
		0x03bf                                                        ,
		0x0275                                                        ,
		0xffd3                                                        ,
		0xfdbe                                                        ,
		0xfd63                                                        ,
		0xfec2                                                        ,
		0x00b5                                                        ,
		0x0200                                                        ,
		0x01d9                                                        ,
		0x008b                                                        ,
		0xfecf                                                        ,
		0xfd9f                                                        ,
		0xfd4d                                                        ,
		0xfdde                                                        ,
		0xfeb5                                                        ,
		0xff7d
};

/* these globals are written by interrupt service routines; we have to declare 
 * these as volatile to avoid the compiler caching their values in registers */
extern volatile unsigned char byte1, byte2, byte3;	/* modified by PS/2 interrupt service routine */
extern volatile int buf_index_record, buf_index_play;
extern volatile unsigned int l_buf[BUF_SIZE];					// audio buffer
extern volatile unsigned int r_buf[BUF_SIZE];					// audio buffer
extern volatile unsigned int Echo_buf_l[BUF_SIZE];
extern volatile unsigned int Echo_buf_r[BUF_SIZE];
extern volatile unsigned int Echo_complete;
extern unsigned int Click_Echo;
extern unsigned int End_Record;

extern int Play_Index;
extern int Play_Flag;
extern int Clear_Play_Plot;


unsigned int nibble_TempY,nibble_TempX,Temp_Sign,Xsign,Ysign,XoverFlow,YoverFlow,LeftBtn,RightBtn,MiddleBtn;
extern int count;
extern volatile int timeout;								// used to synchronize with the timer
extern struct alt_up_dev up_dev;							/* pointer to struct that holds pointers to
																		open devices */
long long int Average[N];
int index_Echo;
int Echo = 0;

int Mouse_Icon[16][8] = {
		{0,-1,-1,-1,-1,-1,-1,-1},
		{0,0,-1,-1,-1,-1,-1,-1},
		{0,1,0,-1,-1,-1,-1,-1},
		{0,1,1,0,-1,-1,-1,-1},
		{0,1,1,1,0,-1,-1,-1},
		{0,1,1,1,1,0,-1,-1},
		{0,1,1,1,1,1,0,-1},
		{0,1,1,1,1,0,0,0},
		{0,1,1,1,0,-1,-1,-1},
		{0,0,0,1,0,-1,-1,-1},
		{0,-1,0,1,0,-1,-1,-1},
		{-1,-1,-1,0,1,0,-1,-1},
		{-1,-1,-1,0,1,0,-1,-1},
		{-1,-1,-1,-1,0,1,0,-1},
		{-1,-1,-1,-1,0,1,0,-1},
		{-1,-1,-1,-1,-1,0,-1,-1},
};

int anita0 = 0;
int anita1 = 0;
int anita2 = 0;
/* function prototypes */
void interval_timer_ISR(void *, unsigned int);
void pushbutton_ISR(void *, unsigned int);
void audio_ISR(void *, unsigned int);
void PS2_ISR(void *, unsigned int);
void HEXxy_PS2(struct alt_up_dev *,unsigned char , unsigned char , unsigned char );
//void Do_Hard_Echo();
void Do_Soft_Echo();
void Plot_AudioRecord(alt_up_pixel_buffer_dma_dev *);
void Audio_Average();
void Plot_Play_Audio(alt_up_pixel_buffer_dma_dev *);

/********************************************************************************
 * This program demonstrates use of the media ports in the DE2 Media Computer
 *
 * It performs the following: 
 *  	1. records audio for about 10 seconds when an interrupt is generated by
 *  	   pressing KEY[1]. LEDG[0] is lit while recording. Audio recording is 
 *  	   controlled by using interrupts
 * 	2. plays the recorded audio when an interrupt is generated by pressing
 * 	   KEY[2]. LEDG[1] is lit while playing. Audio playback is controlled by 
 * 	   using interrupts
 * 	3. Draws a blue box on the VGA display, and places a text string inside
 * 	   the box. Also, moves the word ALTERA around the display, "bouncing" off
 * 	   the blue box and screen edges
 * 	4. Shows a text message on the LCD display, and scrolls the message
 * 	5. Displays the last three bytes of data received from the PS/2 port 
 * 	   on the HEX displays on the DE2 board. The PS/2 port is handled using 
 * 	   interrupts
 * 	6. The speed of scrolling the LCD display and of refreshing the VGA screen
 * 	   are controlled by interrupts from the interval timer
********************************************************************************/
int main(void)
{
	/* declare device driver pointers for devices */
	alt_up_parallel_port_dev *KEY_dev;
	alt_up_parallel_port_dev *green_LEDs_dev;
	alt_up_parallel_port_dev *red_LEDs_dev;
	alt_up_ps2_dev *PS2_dev;
	alt_up_character_lcd_dev *lcd_dev;
	alt_up_audio_dev *audio_dev;
	alt_up_char_buffer_dev *char_buffer_dev;
	alt_up_pixel_buffer_dma_dev *pixel_buffer_dev;
	/* declare volatile pointer for interval timer, which does not have HAL functions */
	volatile int * interval_timer_ptr = (int *) 0x10002000;	// interal timer base address

	/* initialize some variables */
	byte1 = 0; byte2 = 0; byte3 = 0; 			// used to hold PS/2 data
	count = 0;
	timeout = 0;										// synchronize with the timer

	/* these variables are used for a blue box and a "bouncing" ALTERA on the VGA screen */
	int MouseX; int MouseY;
	int Temp_MouseX; int Temp_MouseY;
	int blue_x1; int blue_y1; int blue_x2; int blue_y2; 
	int screen_x; int screen_y; int char_buffer_x; int char_buffer_y;
	short color;

	/* set the interval timer period for scrolling the HEX displays */
	int counter = 0x960000;				// 1/(50 MHz) x (0x960000) ~= 200 msec
	*(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
	*(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;

	/* start interval timer, enable its interrupts */
	*(interval_timer_ptr + 1) = 0x7;	// STOP = 0, START = 1, CONT = 1, ITO = 1 
	
	// open the pushbuttom KEY parallel port
	KEY_dev = alt_up_parallel_port_open_dev ("/dev/Pushbuttons");
	if ( KEY_dev == NULL)
	{
		alt_printf ("Error: could not open pushbutton KEY device\n");
		return -1;
	}
	else
	{
		alt_printf ("Opened pushbutton KEY device\n");
		up_dev.KEY_dev = KEY_dev;	// store for use by ISRs
	}
	/* write to the pushbutton interrupt mask register, and set 3 mask bits to 1 
	 * (bit 0 is Nios II reset) */
	alt_up_parallel_port_set_interrupt_mask (KEY_dev, 0xE);

	// open the green LEDs parallel port
	green_LEDs_dev = alt_up_parallel_port_open_dev ("/dev/Green_LEDs");
	if ( green_LEDs_dev == NULL)
	{
		alt_printf ("Error: could not open green LEDs device\n");
		return -1;
	}
	else
	{
		alt_printf ("Opened green LEDs device\n");
		up_dev.green_LEDs_dev = green_LEDs_dev;	// store for use by ISRs
	}

	// open the red LEDs parallel port
	green_LEDs_dev = alt_up_parallel_port_open_dev ("/dev/Red_LEDs");
	if ( green_LEDs_dev == NULL)
	{
		alt_printf ("Error: could not open red LEDs device\n");
		return -1;
	}
	else
	{
		alt_printf ("Opened red LEDs device\n");
		up_dev.red_LEDs_dev = red_LEDs_dev;	// store for use by ISRs
	}

	// open the PS2 port
	PS2_dev = alt_up_ps2_open_dev ("/dev/PS2_Port");
	if ( PS2_dev == NULL)
	{
		alt_printf ("Error: could not open PS2 device\n");
		return -1;
	}
	else
	{
		alt_printf ("Opened PS2 device\n");
		up_dev.PS2_dev = PS2_dev;	// store for use by ISRs
	}
	(void) alt_up_ps2_write_data_byte (PS2_dev, 0xFF);		// reset
	alt_up_ps2_enable_read_interrupt (PS2_dev); // enable interrupts from PS/2 port

	// open the audio port
	audio_dev = alt_up_audio_open_dev ("/dev/Audio");
	if ( audio_dev == NULL)
	{
		alt_printf ("Error: could not open audio device\n");
		return -1;
	}
	else
	{
		alt_printf ("Opened audio device\n");
		up_dev.audio_dev = audio_dev;	// store for use by ISRs
	}

	// open the 16x2 character display port
	lcd_dev = alt_up_character_lcd_open_dev ("/dev/Char_LCD_16x2");
	if ( lcd_dev == NULL)
	{
		alt_printf ("Error: could not open character LCD device\n");
		return -1;
	}
	else
	{
		alt_printf ("Opened character LCD device\n");
		up_dev.lcd_dev = lcd_dev;	// store for use by ISRs
	}

	/* use the HAL facility for registering interrupt service routines. */
	/* Note: we are passsing a pointer to up_dev to each ISR (using the context argument) as 
	 * a way of giving the ISR a pointer to every open device. This is useful because some of the
	 * ISRs need to access more than just one device (e.g. the pushbutton ISR accesses both
	 * the pushbutton device and the audio device) */
	alt_irq_register (0, (void *) &up_dev, (void *) interval_timer_ISR);
	alt_irq_register (1, (void *) &up_dev, (void *) pushbutton_ISR);
	alt_irq_register (6, (void *) &up_dev, (void *) audio_ISR);
	alt_irq_register (7, (void *) &up_dev, (void *) PS2_ISR);

	/* create a messages to be displayed on the VGA and LCD displays */
	char text_top_LCD[80] = "Welcome to the DE2 Media Computer...\0";
	char text_Record[20] = "Record\0";
	char text_Play[20] = "Play\0";
	char text_Echo[20] = "Echo\0";
	char test_Audio[20] = "Audio Plot\0";

	/* output text message to the LCD */
	alt_up_character_lcd_set_cursor_pos (lcd_dev, 0, 0);	// set LCD cursor location to top row
	alt_up_character_lcd_string (lcd_dev, text_top_LCD);
	alt_up_character_lcd_cursor_off (lcd_dev);				// turn off the LCD cursor 

	/* open the pixel buffer */
	pixel_buffer_dev = alt_up_pixel_buffer_dma_open_dev ("/dev/VGA_Pixel_Buffer");
	if ( pixel_buffer_dev == NULL)
		alt_printf ("Error: could not open pixel buffer device\n");
	else
		alt_printf ("Opened pixel buffer device\n");

	/* output text message in the middle of the VGA monitor */
	char_buffer_dev = alt_up_char_buffer_open_dev ("/dev/VGA_Char_Buffer");
	if ( char_buffer_dev == NULL)
		alt_printf ("Error: could not open character buffer device\n");
	else
		alt_printf ("Opened character buffer device\n");


	/* the following variables give the size of the pixel buffer */
	screen_x = 319; screen_y = 239;
	color = 0x1863;		// a dark grey color
	alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, 0, 0, screen_x, 
		screen_y, color, 0); // fill the screen
	
	// draw a medium-blue box in the middle of the screen, using character buffer coordinates
	blue_x1 = 0; blue_x2 = 100; blue_y1 = 0; blue_y2 = 20;
	// character coords * 4 since characters are 4 x 4 pixel buffer coords (8 x 8 VGA coords)
	color = 0x0360;		// a medium blue color
	alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, blue_x1 * 4, blue_y1 * 4, blue_x2 * 4, 
		blue_y2 * 4, color, 0);

	//Green Box For Record //Record ******************
	blue_x1 = 15; blue_x2 = 25; blue_y1 = 4; blue_y2 = 14;
	// character coords * 4 since characters are 4 x 4 pixel buffer coords (8 x 8 VGA coords)
	color = 0x100F;		// a medium blue color
	alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, blue_x1 * 4, blue_y1 * 4, blue_x2 * 4,
		blue_y2 * 4, color, 0);
	alt_up_char_buffer_string (char_buffer_dev, text_Record, blue_x1 + 2, blue_y1 + 4);

	//Green Box For Play //PLAY**********************
	blue_x1 = 35; blue_x2 = 45; blue_y1 = 4; blue_y2 = 14;
	// character coords * 4 since characters are 4 x 4 pixel buffer coords (8 x 8 VGA coords)
	color = 0x100F;		// a medium blue color
	alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, blue_x1 * 4, blue_y1 * 4, blue_x2 * 4,
		blue_y2 * 4, color, 0);
	alt_up_char_buffer_string (char_buffer_dev, text_Play, blue_x1 + 4, blue_y1 + 4);

	//Green Box For Echo //Echo**********************
	blue_x1 = 55; blue_x2 = 65; blue_y1 = 4; blue_y2 = 14;
	// character coords * 4 since characters are 4 x 4 pixel buffer coords (8 x 8 VGA coords)
	color = 0x100F;		// a medium blue color
	alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, blue_x1 * 4, blue_y1 * 4, blue_x2 * 4,
		blue_y2 * 4, color, 0);
	alt_up_char_buffer_string (char_buffer_dev, text_Echo, blue_x1 + 4, blue_y1 + 4);

	// Draw Audio Display
	blue_x1 = 0; blue_x2 = 100; blue_y1 = 20; blue_y2 = 60;
	// character coords * 4 since characters are 4 x 4 pixel buffer coords (8 x 8 VGA coords)
	color = 0x0328;		// a medium blue color
	alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, blue_x1 * 4, blue_y1 * 4, blue_x2 * 4,
		blue_y2 * 4, color, 0);

	//Write Audio
	blue_x1 = 39; blue_y1 = 22;
	color = 0xABCD;
	alt_up_char_buffer_string (char_buffer_dev, test_Audio, blue_x1 , blue_y1 );
	//Audio Line
	blue_x1 = 10; blue_x2 = 70; blue_y1 = 21;
	color = 0xABCD;
	alt_up_pixel_buffer_dma_draw_hline(pixel_buffer_dev,blue_x1 * 4, blue_x2 * 4, blue_y1 * 4, color, 0);
	blue_x1 = 10; blue_x2 = 70; blue_y1 = 23;
	alt_up_pixel_buffer_dma_draw_hline(pixel_buffer_dev,blue_x1 * 4, blue_x2 * 4, blue_y1 * 4, color, 0);
	//Audio OverFlow
	alt_up_pixel_buffer_dma_draw_hline(pixel_buffer_dev,10, 300, 105, 0xABCD, 0);

	
	char_buffer_x = 79; char_buffer_y = 59;
	MouseX = 10;
	MouseY = 10;
	int temp1;
	int temp2;
	int temp3;
	Echo_complete = 0;
	Play_Index = 0;
	/* this loops "bounces" the word ALTERA around on the VGA screen */
	while (1)
	{
			while(!timeout);
			// wait to synchronize with timeout, which is set by the interval timer ISR
			/* also, display any PS/2 data (from its interrupt service routine) on HEX displays */
			HEXxy_PS2 (&up_dev, byte1, byte2, byte3);
			temp1 = byte1;
			temp2 = byte2;
			temp3 = byte3;
			if((temp1 >= 100 && !Xsign) || (temp2>= 100 && !Ysign)){
				continue;
			}
			if((temp1 <= 150 && Xsign) || (temp2<= 150 && Ysign)){
				continue;
			}


			//Record interrupt
			if(Temp_MouseX >= 60 && Temp_MouseX <= 100 && Temp_MouseY >= 16 && Temp_MouseY <= 56){
				if(Temp_MouseX <= 62 || Temp_MouseX >= 98 || Temp_MouseY <= 20 || Temp_MouseY >= 52){

					// draw a medium-blue box in the middle of the screen, using character buffer coordinates
					blue_x1 = 14; blue_x2 = 26; blue_y1 = 3; blue_y2 = 15;
					// character coords * 4 since characters are 4 x 4 pixel buffer coords (8 x 8 VGA coords)
					color = 0x0360;		// a medium blue color
					alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, blue_x1 * 4, blue_y1 * 4, blue_x2 * 4,
						blue_y2 * 4, color, 0);

					//Green Box For Record //Record ******************
					// character coords * 4 since characters are 4 x 4 pixel buffer coords (8 x 8 VGA coords)
					blue_x1 = 15; blue_x2 = 25; blue_y1 = 4; blue_y2 = 14;
					color = 0x100F;
					alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, blue_x1 * 4, blue_y1 * 4, blue_x2 * 4,
						blue_y2 * 4, color, 0);
					alt_up_char_buffer_string (char_buffer_dev, text_Record, blue_x1 + 2, blue_y1 + 4);

				}
				else {
					color = 0x100F;		// a medium blue color
					alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, Temp_MouseX - 2, Temp_MouseY - 2, Temp_MouseX + 2, Temp_MouseY + 2, color, 1);
				}
			}


			else if(Temp_MouseX >= 140 && Temp_MouseX <= 180 && Temp_MouseY >= 16 && Temp_MouseY <= 56){
				if(Temp_MouseX <= 142 || Temp_MouseX >= 178 || Temp_MouseY <= 20 || Temp_MouseY >= 52){

					// draw a medium-blue box in the middle of the screen, using character buffer coordinates
					blue_x1 = 34; blue_x2 = 46; blue_y1 = 3; blue_y2 = 15;
					// character coords * 4 since characters are 4 x 4 pixel buffer coords (8 x 8 VGA coords)
					color = 0x0360;		// a medium blue color
					alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, blue_x1 * 4, blue_y1 * 4, blue_x2 * 4,
						blue_y2 * 4, color, 0);

					blue_x1 = 35; blue_x2 = 45; blue_y1 = 4; blue_y2 = 14;
					//Green Box For Record //Record ******************
					// character coords * 4 since characters are 4 x 4 pixel buffer coords (8 x 8 VGA coords)
					color = 0x100F;		//
					alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, blue_x1 * 4, blue_y1 * 4, blue_x2 * 4,
						blue_y2 * 4, color, 0);
					alt_up_char_buffer_string (char_buffer_dev, text_Play, blue_x1 + 4, blue_y1 + 4);

				}
				else {
					color = 0x100F;		// a medium blue color
					alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, Temp_MouseX - 2, Temp_MouseY - 2, Temp_MouseX + 2, Temp_MouseY + 2, color, 1);
				}
			}

			else if(Temp_MouseX >= 220 && Temp_MouseX <= 260 && Temp_MouseY >= 16 && Temp_MouseY <= 56){
				if(Temp_MouseX <= 222 || Temp_MouseX >= 258 || Temp_MouseY <= 20 || Temp_MouseY >= 52){

					// draw a medium-blue box in the middle of the screen, using character buffer coordinates
					blue_x1 = 54; blue_x2 = 66; blue_y1 = 3; blue_y2 = 15;
					// character coords * 4 since characters are 4 x 4 pixel buffer coords (8 x 8 VGA coords)
					color = 0x0360;		// a medium blue color
					alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, blue_x1 * 4, blue_y1 * 4, blue_x2 * 4,
						blue_y2 * 4, color, 0);

					//Green Box For Record //Record ******************
					// character coords * 4 since characters are 4 x 4 pixel buffer coords (8 x 8 VGA coords)
					blue_x1 = 55; blue_x2 = 65; blue_y1 = 4; blue_y2 = 14;
					color = 0x100F;
					alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, blue_x1 * 4, blue_y1 * 4, blue_x2 * 4,
						blue_y2 * 4, color, 0);
					alt_up_char_buffer_string (char_buffer_dev, text_Echo, blue_x1 + 4, blue_y1 + 4);

				}
				else {
					color = 0x100F;		// a medium blue color
					alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, Temp_MouseX - 2, Temp_MouseY - 2, Temp_MouseX + 2, Temp_MouseY + 2, color, 1);
				}
			}

			//Boxe Out
			else if(Temp_MouseX >= 0 && Temp_MouseX <= 400 && Temp_MouseY >= 0 && Temp_MouseY <= 80){
				color = 0x0360;
				alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, Temp_MouseX - 2, Temp_MouseY - 2, Temp_MouseX + 2, Temp_MouseY + 2, color, 1);
			}

//			else {
//				color = 0x1863;
//				alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, Temp_MouseX - 2, Temp_MouseY - 2, Temp_MouseX + 2, Temp_MouseY + 2, color, 1);
//			}

//			alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, Temp_MouseX - 2, Temp_MouseY - 2, Temp_MouseX + 2, Temp_MouseY + 2, color, 1);

			if(Xsign){
				MouseX = MouseX - (256-(alt_u8)(temp1)) - 1;
			}
			else{
				MouseX = MouseX + (alt_u8)(temp1);
			}
			if(Ysign){
				MouseY = MouseY + (256-(alt_u8)(temp2)) + 1;
			}
			else {
				MouseY = MouseY - (alt_u8)(temp2);
			}
			if(MouseX >= 320){
				MouseX = 320;
			}
			if(MouseX <= 2){
				MouseX = 2;
			}
			if(MouseY >= 77){
				MouseY = 77;
			}
			if(MouseY <= 2){
				MouseY = 2;
			}
			color = 0xDDDD;		// a medium blue color
			alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, MouseX - 2, MouseY - 2, MouseX + 2, MouseY + 2, color, 1);
//			int a;
//			int b;
//			for(a=0;a<16;a++){
//				for(b=0;b<8;b++){
//					if(Mouse_Icon[a][b] == -1){
//						alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, MouseX - a, MouseY - b,MouseX - a+1, MouseY - b+1, color, 1);
//					}
//					if(Mouse_Icon[a][b] == 0){
//						color = 0x0;
//						alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, MouseX - a, MouseY - b,MouseX - a+1, MouseY - b+1, color, 1);
//					}
//					if(Mouse_Icon[a][b] == 1){
//						color = 0xFFFF;
//						alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, MouseX - a, MouseY - b,MouseX - a+1, MouseY - b+1, color, 1);
//					}
//				}
//			}

			Temp_MouseX = MouseX;
			Temp_MouseY = MouseY;

			//Record interrupt
			if(MouseX >= 60 && MouseX <= 100 && MouseY >= 16 && MouseY <= 56 && LeftBtn){
				index_Echo = 0;
				Echo_complete = 0;
				Click_Echo = 0;
				// reset the buffer index for recording
				buf_index_record = 0;
				// clear audio FIFOs
				alt_up_audio_reset_audio_core (audio_dev);
				// enable audio-in interrupts
				alt_up_audio_enable_read_interrupt (audio_dev);
			}

			//Play interrupt
			if(MouseX >= 140 && MouseX <= 180 && MouseY >= 16 && MouseY <= 56 && LeftBtn){
				Click_Echo = 0;
				Play_Index = 0;
				alt_up_pixel_buffer_dma_draw_box (pixel_buffer_dev, 0 , 200, 100 * 4,
					250, 0x0328, 0);
				// reset the buffer index for recording
				buf_index_play = 0;
				// clear audio FIFOs
				alt_up_audio_reset_audio_core (audio_dev);
				// enable audio-in interrupts
				alt_up_audio_enable_write_interrupt (audio_dev);
			}

			//Echo interrupt
			if(MouseX >= 220 && MouseX <= 260 && MouseY >= 16 && MouseY <= 56 && LeftBtn){
				Do_Soft_Echo();
				Click_Echo = 1;
				// reset the buffer index for recording
				buf_index_play = 0;
				// clear audio FIFOs
				alt_up_audio_reset_audio_core (audio_dev);
				// enable audio-in interrupts
				alt_up_audio_enable_write_interrupt (audio_dev);
			}
//			if(Echo_End == 1 && Echo_complete == 0){
//				Do_Echo();
//			}

			if(End_Record == 1){
				Audio_Average();
				Plot_AudioRecord(pixel_buffer_dev);
				End_Record = 0;
			}
			Plot_Play_Audio(pixel_buffer_dev);
			timeout = 0;

		}
}

/****************************************************************************************
 * Subroutine to show a string of HEX data on the HEX displays
 * Note that we are using pointer accesses for the HEX displays parallel port. We could
 * also use the HAL functions for these ports instead
 ****************************************************************************************/

/****************************************************************************************/
void HEXxy_PS2(struct alt_up_dev *up_dev,unsigned char b1, unsigned char b2, unsigned char b3)
{
	volatile int *HEX3_HEX0_ptr = (int *) 0x10000020;
	volatile int *HEX7_HEX4_ptr = (int *) 0x10000030;

	/* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
	 * a single 7-seg display in the DE2 Media Computer, for the hex digits 0 - F */
	unsigned char	seven_seg_decode_table[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
												0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71  };
	unsigned char	hex_segs[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned int shift_buffer,Temp_Buffer, nibble;
	unsigned char code;
	unsigned int Mouse_Btn;
	int i;

	shift_buffer = (b1 << 16) | (b2 << 8) | b3;
	Temp_Buffer = shift_buffer;


	for ( i = 0; i < 6; i++ )
	{
		nibble = shift_buffer & 0x0000000F;		// character is in rightmost nibble
		code = seven_seg_decode_table[nibble];
		if(i==0){
			LeftBtn   = shift_buffer & 0x00000001;
			RightBtn  = shift_buffer & 0x00000002;
			MiddleBtn = shift_buffer & 0x00000004;
			Mouse_Btn = LeftBtn | MiddleBtn | RightBtn;
			IOWR_ALT_UP_PARALLEL_PORT_DATA(RED_LEDS_BASE,Mouse_Btn);
		}
		else if(i==1){
			Xsign = shift_buffer & 0x00000001;
			Ysign = shift_buffer & 0x00000002;
			XoverFlow = shift_buffer & 0x00000004;
			YoverFlow = shift_buffer & 0x00000008;
			hex_segs[6] = seven_seg_decode_table[Xsign];
			hex_segs[2] = seven_seg_decode_table[Ysign];
			hex_segs[7] = seven_seg_decode_table[XoverFlow];
			hex_segs[3] = seven_seg_decode_table[YoverFlow];
		}
		else if(i==4 || i==5){
			Temp_Sign = Temp_Buffer & 0x00000010;
			if(Xsign){
				nibble_TempX = 15 - nibble;
				code = seven_seg_decode_table[nibble_TempX];
			}
			else{
				code = seven_seg_decode_table[nibble];
			}
			hex_segs[i] = code;
		}
		else if(i==2 || i==3){
			Temp_Sign = Temp_Buffer & 0x00000002;
			if(Ysign){
				nibble_TempY = 15 - nibble;
				code = seven_seg_decode_table[nibble_TempY];
			}
			else{
				code = seven_seg_decode_table[nibble];
			}
			hex_segs[i-2] = code;
		}
		shift_buffer = shift_buffer >> 4;
	}
	/* drive the hex displays */
	*(HEX3_HEX0_ptr) = *(int *) (hex_segs);
	*(HEX7_HEX4_ptr) = *(int *) (hex_segs+4);
}
/****************************************************************************************/
//
//void Do_Echo(){
//	IOWR_ALT_UP_PARALLEL_PORT_DATA(GREEN_LEDS_BASE,0x10);
//		while(index_Echo <= BUF_SIZE+5){
//				Echo_buf_l[index_Echo] = 0.25 * l_buf[index_Echo-5000] + 0.25 * l_buf[index_Echo-1000] + 0.5 * l_buf[index_Echo];
//				Echo_buf_r[index_Echo] = 0.25 * r_buf[index_Echo-5000] + 0.25 * r_buf[index_Echo-1000] + 0.5 * r_buf[index_Echo];
//				index_Echo++;
//				if(index_Echo == BUF_SIZE){
//					IOWR_ALT_UP_PARALLEL_PORT_DATA(GREEN_LEDS_BASE,0x0);
//					Echo = 0;
//					Echo_complete = 1;
//					break;
//				}
//		}
//}
/****************************************************************************************/
//void Do_Hard_Echo(){
//	int i;
//
//	for(i=0;i<BUF_SIZE;i++){
//		Echo_buf_l[i] = ALT_CI_FIR_NEW_0(l_buf[i]) << 8;
//	}
//	for(i=0;i<BUF_SIZE;i++){
//		Echo_buf_r[i] = ALT_CI_FIR_NEW_0(r_buf[i]) << 8;
//	}
//
//	IOWR_ALT_UP_PARALLEL_PORT_DATA(GREEN_LEDS_BASE,0x0);
//	Echo = 0;
//	Echo_complete = 1;
//
//}


/****************************************************************************************/
void Do_Soft_Echo(){
	int j;
	int i;
	for(i=0;i<BUF_SIZE;i++){
		alt_64 temp = 0;
		for(j = 0; j<64; j++)
		{
			alt_64 eff = (alt_64)(coeff[j]);
			alt_64 data = ((alt_64)((alt_32)l_buf[i-j]));
			temp = (temp + (data * eff));
		}
		temp = (temp >> 15);
		alt_u32 dataout = (alt_u32)((alt_32)temp);
		Echo_buf_l[i] = dataout;
		Echo_buf_r[i] = dataout;
	}

//	for(int i=0;i<BUF_SIZE;i++){
//		alt_64 temp;
//		for(int j = 0; j<64; j++)
//		{
//			temp = ((alt_64)temp + (((alt_64)((alt_32)r_buf[i])) * ((alt_64)coeff)));
//		}
//		Echo_buf_r[i] = temp;
//	}

	IOWR_ALT_UP_PARALLEL_PORT_DATA(GREEN_LEDS_BASE,0x0);
	Echo = 0;
	Echo_complete = 1;
}

/****************************************************************************************/
void Plot_AudioRecord(alt_up_pixel_buffer_dma_dev *pixel_buffer){

	// Draw Audio Display
	// character coords * 4 since characters are 4 x 4 pixel buffer coords (8 x 8 VGA coords)
	alt_up_pixel_buffer_dma_draw_box (pixel_buffer, 0 , 20 * 4, 100 * 4,
		60 * 4, 0x0328, 0);

	//Audio Line
	alt_up_pixel_buffer_dma_draw_hline(pixel_buffer,10 * 4, 70 * 4, 21 * 4, 0xABCD, 0);
	alt_up_pixel_buffer_dma_draw_hline(pixel_buffer,10 * 4, 70 * 4, 23 * 4, 0xABCD, 0);
	//Audio OverFlow
	alt_up_pixel_buffer_dma_draw_hline(pixel_buffer,10, 300, 105, 0xABCD, 0);

	int i=0;
	short color = 0x8053;
	int x1 = 10;
	int x2 = 310;
	int gap = 3;
	int Range = ((x2 - x1)/N)-3;

	int x_Start=0,x_End=10;

	for(i=0;i<N;i++){
		x_Start = x_End + gap;
		x_End = x_Start + Range;
		//Draw Audio Range
		alt_up_pixel_buffer_dma_draw_box (pixel_buffer, x_Start, 200-Average[i], x_End,
			200, color, 0);
	}
}
/****************************************************************************************/
void Audio_Average(){
	int i=0;
	int j=0;
	long long int Sum=0;

	int anita;

	int Duration = BUF_SIZE / N;
	for(j=0;j<N;j++){
		Average[j] = 0;
		Sum =0;
		for(i=0;i<Duration;i++){
			Sum = ((alt_u64)abs(((alt_u32)l_buf[i + j * Duration]))) + Sum;
			Sum = ((alt_u64)abs(((alt_u32)r_buf[i + j * Duration]))) + Sum;
		}

		Average[j] = (((Sum * 400) / (Duration) ) >> 32);
		anita = Average[j];
        if((200-Average[j]) < 110 ){
        	Average[j] = 95;
        }
	}
}
/****************************************************************************************/
void Plot_Play_Audio(alt_up_pixel_buffer_dma_dev *pixel_buffer){
	int x_Start=0,x_End=10;
	int gap = 3;
	int x1 = 10;
	int x2 = 310;
	int Range = ((x2 - x1)/N)-3;
	short color = 0x1111;


	if(Play_Flag == 1){
		x_Start = Play_Index * Range + gap * Play_Index + 13;
		x_End = x_Start + Range;
		Play_Flag = 0;
		Play_Index = Play_Index + 1;
		alt_up_pixel_buffer_dma_draw_box (pixel_buffer, x_Start, 205, x_End,
			205 + Range , color, 0);
	}

}

