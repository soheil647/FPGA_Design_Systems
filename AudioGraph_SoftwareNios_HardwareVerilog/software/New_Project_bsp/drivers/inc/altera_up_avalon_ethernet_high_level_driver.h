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

#ifndef __ALT_UP_ETHERNET_HIGH_LEVEL_DRIVERS_H_
#define __ALT_UP_ETHERNET_HIGH_LEVEL_DRIVERS_H_

#include "altera_up_avalon_ethernet.h"

// Interrupt Mask Macros
#define ALT_UP_ETHERNET_INTERRUPT_MASK_LNKCHGI	0x20	// Packet Link Status Change Interrupt
#define ALT_UP_ETHERNET_INTERRUPT_MASK_UDRUNI	0x10	// Packet Transmit Underrun Interrupt
#define ALT_UP_ETHERNET_INTERRUPT_MASK_ROOI		0x08	// Packet Receive Overflow Counter Overflow Interrupt
#define ALT_UP_ETHERNET_INTERRUPT_MASK_ROI		0x04	// Packet Receive Overflow Interrupt
#define ALT_UP_ETHERNET_INTERRUPT_MASK_PTI		0x02	// Packet Transmitted Interrupt
#define ALT_UP_ETHERNET_INTERRUPT_MASK_PRI		0x01	// Packet Received Interrupt
#define ALT_UP_ETHERNET_INTERRUPT_MASK_NONE		0x00	// No Interrupts

// Packet transmit status
#define ALT_UP_ETHERNET_TX_STATUS_OK					0		// Packet transmitted successfully
#define ALT_UP_ETHERNET_TX_STATUS_TIMEOUT				-1		// Packet transmission timed out

// Packet received status mask
#define ALT_UP_ETHERNET_RX_STATUS_RUNT					0x80	// Received packet is a runt packet
#define ALT_UP_ETHERNET_RX_STATUS_MULTICAST				0x40	// Received packet is a multicast packet
#define ALT_UP_ETHERNET_RX_STATUS_LATE_COLLISION		0x20	// Received packet has a late collision error
#define ALT_UP_ETHERNET_RX_STATUS_WATCHDOG_TIMEOUT		0x10	// Received packet has a watchdog timeout error
#define ALT_UP_ETHERNET_RX_STATUS_PHY_LAYER_ERROR		0x08	// Received packet has a physical layer error
#define ALT_UP_ETHERNET_RX_STATUS_ALIGN_ERROR			0x04	// Received packet has an alignment error
#define ALT_UP_ETHERNET_RX_STATUS_CRC_ERROR				0x02	// Received packet has a crc error
#define ALT_UP_ETHERNET_RX_STATUS_FIFO_OVERFLOW_ERROR	0x01	// Receive fifo overflow error
#define ALT_UP_ETHERNET_RX_STATUS_OK					0x00	// Packet received is good
#define ALT_UP_ETHERNET_RX_STATUS_BAD_PACKET			-3		// Bad packet
#define ALT_UP_ETHERNET_RX_STATUS_RESET_NIC				-2		// NIC was reset
#define ALT_UP_ETHERNET_RX_STATUS_NO_PACKET				-1		// Receive buffer is empty


/**
 * @brief Sends an Ethernet packet
 *
 * @param ethernet_dev -- the Ethernet device structure
 * @param data_ptr -- pointer to the location of the packet
 * @param data_length -- packet length
 *
 * @return 0
 **/
int alt_up_ethernet_send_pkt (alt_up_ethernet_dev * ethernet_dev, unsigned char * data_ptr, unsigned int pkt_length);

/**
 * @brief Reads an incoming Ethernet packet
 *
 * @param ethernet_dev -- the Ethernet device structure
 * @param data_ptr -- pointer to the location of the packet
 * @param pkt_length -- pointer to the packet length
 *
 * @return 0 for success
 * @return -1 if sending timed out
 **/
int alt_up_ethernet_receive_pkt (alt_up_ethernet_dev * ethernet_dev, unsigned char * data_ptr, unsigned int * pkt_length);

/**
 * @brief Sets the Interrupt Mask register (IMR) of the ethernet chip.
 *
 * @param ethernet_dev -- the Ethernet device structure
 * @param interrupts -- A mask that indicated which interrupts are to be enabled
 *
 * @return 0 for success
 * @return -1 if the rx buffer is empty
 * @return -2 if the chip had to be reset to recover from an unstable state
 * @return -3 if the packet was bad, and was flushed without reading
 **/
unsigned int alt_up_ethernet_set_interrupts (alt_up_ethernet_dev * ethernet_dev, unsigned int interrupts);

#endif  /* __ALT_UP_ETHERNET_HIGH_LEVEL_DRIVERS_H_ */

