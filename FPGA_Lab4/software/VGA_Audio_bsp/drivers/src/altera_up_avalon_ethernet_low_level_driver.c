/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2007 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
*                                                                             *
******************************************************************************/

#include "altera_up_avalon_ethernet_low_level_driver.h"
#include "altera_up_avalon_ethernet_regs.h"
#include <unistd.h>

unsigned int alt_up_ethernet_reg_read(unsigned int base, unsigned int reg)
{
	IOWR_ALTERA_UP_AVALON_ETHERNET_INDEX(base, reg);
	return IORD_ALTERA_UP_AVALON_ETHERNET_DATA(base);
}

void alt_up_ethernet_reg_write(unsigned int base, unsigned int reg, unsigned int data)
{
	IOWR_ALTERA_UP_AVALON_ETHERNET_INDEX(base, reg);
	IOWR_ALTERA_UP_AVALON_ETHERNET_DATA(base, data);
}

unsigned int alt_up_ethernet_phy_reg_read (unsigned int base, unsigned int reg)
{
	unsigned int result = 0;
	unsigned int timeout = 0;

	/* set PHY register address into EPAR REG. 0CH */
	alt_up_ethernet_reg_write(base, ALT_UP_ETHERNET_EPAR, reg | 0x40);				/* PHY register address setting, and DM9000_PHY offset = 0x40 */

	/* issue PHY + READ command = 0xC into EPCR REG. 0BH */
	alt_up_ethernet_reg_write(base, ALT_UP_ETHERNET_EPCR, 0x0C);					/* issue PHY + READ command */
	do {
		usleep(1);
		timeout++;
	} while ((timeout < 50) && (alt_up_ethernet_reg_read(base, ALT_UP_ETHERNET_EPCR) & 0x01));
	
	usleep(1);
	alt_up_ethernet_reg_write(base, ALT_UP_ETHERNET_EPCR, 0x08);					/* clear PHY command */

	/* read PHY data from EPDR REG. 0EH & REG. 0DH */
	result = (alt_up_ethernet_reg_read(base, ALT_UP_ETHERNET_EPDRH) << 8);			/* PHY data high_byte */
	result |= alt_up_ethernet_reg_read(base, ALT_UP_ETHERNET_EPDRL);				/* PHY data low_byte */

	return result;
}

void alt_up_ethernet_phy_reg_write (unsigned int base, unsigned int reg, unsigned int data)
{ 
	unsigned int timeout = 0;
	
	/* set PHY register address into EPAR REG. 0CH */
	alt_up_ethernet_reg_write(base, ALT_UP_ETHERNET_EPAR, reg | 0x40);				/* PHY register address setting, and DM9000_PHY offset = 0x40 */

	/* fill PHY WRITE data into EPDR REG. 0EH & REG. 0DH */
	alt_up_ethernet_reg_write(base, ALT_UP_ETHERNET_EPDRH, ((data >> 8) & 0xFF));	/* PHY data high_byte */
	alt_up_ethernet_reg_write(base, ALT_UP_ETHERNET_EPDRL, data & 0xFF);			/* PHY data low_byte */

	/* issue PHY + WRITE command = 0xA into EPCR REG. 0BH */
	alt_up_ethernet_reg_write(base, ALT_UP_ETHERNET_EPCR, 0x0A);					/* issue PHY + WRITE command */
	do {
		usleep(1);
		timeout++;
	} while ((timeout < 50) && (alt_up_ethernet_reg_read(base, ALT_UP_ETHERNET_EPCR) & 0x01));
	
	usleep(1);
	
	alt_up_ethernet_reg_write(base, ALT_UP_ETHERNET_EPCR, 0x08);					/* clear PHY command */
}


