/******************************************************************************
 *                                                                             *
 * License Agreement                                                           *
 *                                                                             *
 * Copyright (c) 1991-2011 Altera Corporation, San Jose, California, USA.      *
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

#include "altera_up_avalon_ethernet_regs.h"
#include "altera_up_avalon_ethernet.h"
#include "altera_up_avalon_ethernet_low_level_driver.h"
#include "altera_up_avalon_ethernet_high_level_driver.h"

#include <unistd.h>

int alt_up_ethernet_send_pkt( alt_up_ethernet_dev * ethernet_dev, unsigned char * data_ptr, unsigned int pkt_length )
{
	unsigned int i;
	unsigned int base = ethernet_dev->base;

	/* mask NIC interrupts IMR: PAR only */
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_IMR,  0x80 );

	/* write transmit data to chip SRAM */
	IOWR_ALTERA_UP_AVALON_ETHERNET_INDEX (base, ALT_UP_ETHERNET_MWCMD);
	for (i = 0; i < pkt_length; i += 2)
	{
		IOWR_ALTERA_UP_AVALON_ETHERNET_DATA (base, (data_ptr[i+1]<<8) | data_ptr[i]);
	}

	/* issue TX packet's length into TXPLH REG. FDH & TXPLL REG. FCH */
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_TXPLH, (pkt_length >> 8) & 0xFF );
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_TXPLL,  pkt_length & 0xFF );

	/* issue TX polling command activated */
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_TCR, ALT_UP_ETHERNET_TX_REQUEST );

	// check whether the packet is transmitted (or timeout)
	int timeout = 0xfff;
	while ( ((alt_up_ethernet_reg_read(base, ALT_UP_ETHERNET_NSR)&(4|8))!=1)
			&& (timeout--))
		;

	/* clear the NSR Register */
  	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_NSR, 0x00 );

	/* re-enable NIC interrupts */
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_IMR, ethernet_dev->interrupt_mask_register );

	return (timeout == 0) ? ALT_UP_ETHERNET_TX_STATUS_TIMEOUT : ALT_UP_ETHERNET_TX_STATUS_OK;
}

int alt_up_ethernet_receive_pkt (alt_up_ethernet_dev * ethernet_dev, unsigned char * data_ptr, unsigned int * pkt_length)
{
	unsigned int i;
	unsigned int base = ethernet_dev->base;

	unsigned char RX_ready;
	unsigned short data_in;

	int pkt_status, retval;
	*pkt_length = 0;

	/* mask NIC interrupts IMR: PAR only */
	alt_up_ethernet_reg_write (base, ALT_UP_ETHERNET_IMR,  0x80);
//	usleep(20);
	
	// clear the RX Interrupt bit
	alt_up_ethernet_reg_write(base, ALT_UP_ETHERNET_ISR, 0x01);

	/* dummy read a byte from MRCMDX REG. F0H */
	RX_ready = alt_up_ethernet_reg_read(base, ALT_UP_ETHERNET_MRCMDX);
	RX_ready = IORD_ALTERA_UP_AVALON_ETHERNET_DATA(base);
    
	if (RX_ready & 0x01) {
		// RX is ready

		/* Initial read with address auto-increment */
		IOWR_ALTERA_UP_AVALON_ETHERNET_INDEX(base, ALT_UP_ETHERNET_MRCMD);

		data_in = IORD_ALTERA_UP_AVALON_ETHERNET_DATA (base); // This reads the first 2 bytes: 0x{status}{01}
		pkt_status = (data_in >> 8) & 0xFF;	//RSR register
        
		//	if (pkt_status & 0x80)	alt_putstr("runt frame!\n");
		//	if (pkt_status & 0x40)	alt_putstr("multicast frame!\n");
		//	if (pkt_status & 0x20)	alt_putstr("Late Collision Seen!\n");
		//	if (pkt_status & 0x10)	alt_putstr("Receive Watchdog timeout!\n");
		//	if (pkt_status & 0x08)	alt_putstr("Physical Layer error!\n");
		//	if (pkt_status & 0x04)	alt_putstr("Alignment Error!\n");
		//	if (pkt_status & 0x02)	alt_putstr("CRC Error!\n");
		//	if (pkt_status & 0x01)	alt_putstr("FIFO Overflow Error!\n");

        // Get the packet length, but subtract 4 to ignore the 4 bytes of frame check sequence that gets added (FCS)
		*pkt_length	= IORD_ALTERA_UP_AVALON_ETHERNET_DATA (base) - 4;

        // Check if good packet
        if ( !(pkt_status & 0xBF) && (*pkt_length < MAX_PACKET_SIZE) )
        {
            /* read 1 received packet from RX SRAM into RX buffer */
            for (i = 0; i < *pkt_length; i += 2)
            {
                data_in = IORD_ALTERA_UP_AVALON_ETHERNET_DATA (base);
                data_ptr[i]			= data_in & 0xFF;
                data_ptr[i + 1]	    = (data_in >> 8) & 0xFF;
            }
            /* dump the 4 BYTE FCS */
            IORD_ALTERA_UP_AVALON_ETHERNET_DATA (base);
            IORD_ALTERA_UP_AVALON_ETHERNET_DATA (base);
            
            // Success
            retval = ALT_UP_ETHERNET_RX_STATUS_OK;
        } else {
            // this packet is bad, dump it from RX SRAM
            // also dump the 4 bytes of FCS (thus the +4)
            for (i = 0; i < *pkt_length + 4; i += 2)
            {
                usleep(10);
                IORD_ALTERA_UP_AVALON_ETHERNET_DATA (base);
            }
            *pkt_length = 0;
            
            // Bad packet
            retval = ALT_UP_ETHERNET_RX_STATUS_BAD_PACKET;
        }
        
	} else if (RX_ready & 0x02) {
    
		/* The ethernet chip needs to be reset */
		alt_up_ethernet_reg_write(base, ALT_UP_ETHERNET_NCR, 0x01);
		usleep(10);

		alt_up_ethernet_init( ethernet_dev );

		retval = ALT_UP_ETHERNET_RX_STATUS_RESET_NIC;

	} else {
		/* The ethernet chip is empty */
		retval = ALT_UP_ETHERNET_RX_STATUS_NO_PACKET;
	}
	
	/* re-enable NIC interrupts */
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_IMR, ethernet_dev->interrupt_mask_register );
	
	return retval;
}

unsigned int alt_up_ethernet_set_interrupts (alt_up_ethernet_dev * ethernet_dev, unsigned int interrupts)
{
	/* Set the IMR */
	alt_up_ethernet_reg_write( ethernet_dev->base, ALT_UP_ETHERNET_IMR, 0x80 | interrupts );

	/* Store IMR value */
	ethernet_dev->interrupt_mask_register = interrupts;

	return 0;
}

