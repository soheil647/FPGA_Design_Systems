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

/*
* Register definitions for the DM9000A Ethernet chip used on the 
* DE2 Development and Education Board
*/

#include <io.h>
#include <errno.h>

//TODO: make all macro with same prefix

#ifndef __ALTERA_UP_AVALON_ETHERNET_REGS_H_
#define __ALTERA_UP_AVALON_ETHERNET_REGS_H_

#define ALTERA_UP_AVALON_ETHERNET_VENDOR_ID					0x0A46
#define ALTERA_UP_AVALON_ETHERNET_PRODUCT_ID				0x9000
#define ALTERA_UP_AVALON_ETHERNET_REVISION					0x18

/*
* There are four banks of registers, each paged in or out depending upon the
* value written to the Bank Select Register
*/

/*
 * Index Register (bus width is 16 bit)
 */
#define ALT_UP_ETHERNET_INDEX_REG			           			0
#define IOADDR_ALTERA_UP_AVALON_ETHERNET_INDEX(base)			__IO_CALC_ADDRESS_NATIVE(base, ALT_UP_ETHERNET_INDEX_REG)      
//#define IOWR_ALTERA_UP_AVALON_ETHERNET_INDEX(base,data)			IOWR_16DIRECT(base, ALT_UP_ETHERNET_INDEX_REG, data)
#define IOWR_ALTERA_UP_AVALON_ETHERNET_INDEX(base,data)			IOWR(base, ALT_UP_ETHERNET_INDEX_REG, data)

/*
 * Data Register (bus width is 16 bit)
 */
#define ALT_UP_ETHERNET_DATA_REG			           	1
#define IOADDR_ALTERA_UP_AVALON_ETHERNET_DATA(base)				__IO_CALC_ADDRESS_NATIVE(base, ALT_UP_ETHERNET_DATA_REG)      
//#define IORD_ALTERA_UP_AVALON_ETHERNET_DATA(base)				IORD_16DIRECT(base, ALT_UP_ETHERNET_DATA_REG)
#define IORD_ALTERA_UP_AVALON_ETHERNET_DATA(base)				IORD(base, ALT_UP_ETHERNET_DATA_REG)
//#define IOWR_ALTERA_UP_AVALON_ETHERNET_DATA(base,data)		IOWR_16DIRECT(base, ALT_UP_ETHERNET_DATA_REG, data)
#define IOWR_ALTERA_UP_AVALON_ETHERNET_DATA(base,data)			IOWR(base, ALT_UP_ETHERNET_DATA_REG, data)

// Vendor Control and Status Register Set
#define ALT_UP_ETHERNET_NCR         	0x00  	// Network  Control Register REG. 00 
#define ALT_UP_ETHERNET_NSR         	0x01  	// Network  Status Register  REG. 01 
#define ALT_UP_ETHERNET_TCR         	0x02  	// Transmit Control Register REG. 02 
#define ALT_UP_ETHERNET_TSR_I   		0x03  	// TX Status Register I 
#define ALT_UP_ETHERNET_TSR_II  		0x04  	// TX Status Register II
#define ALT_UP_ETHERNET_RCR         	0x05  	// Receive  Control Register REG. 05 
#define ALT_UP_ETHERNET_RSR     		0x06	// RX Status Register
#define ALT_UP_ETHERNET_ROCR    		0x07	// Receive Overflow Counter Register
#define ALT_UP_ETHERNET_BPTR    		0x08	// Back Pressure Threshold Register
#define ALT_UP_ETHERNET_FCTR    		0x09	// Flow Control Threshold Register
#define ALT_UP_ETHERNET_FCR    			0x0A	// RX Flow Control Register
#define ALT_UP_ETHERNET_EPCR   			0x0B	// EEPROM & PHY Control Register
#define ALT_UP_ETHERNET_EPAR   			0x0C	// EEPROM & PHY Address Register
#define ALT_UP_ETHERNET_EPDRL  			0x0D	// EEPROM & PHY Low Byte Register
#define ALT_UP_ETHERNET_EPDRH  			0x0E	// EEPROM & PHY High Byte Register
#define ALT_UP_ETHERNET_WCR    			0x0F	// Wake Up Control Register
#define ALT_UP_ETHERNET_PAR    			0x10	// Physical Address Register
#define ALT_UP_ETHERNET_MAR    			0x16	// Multicast Address Register

#define ALT_UP_ETHERNET_GPCR    		0x1E	// General Purpose Control Register (in 8-bit mode)
#define ALT_UP_ETHERNET_GPR    			0x1F	// General Purpose Register

#define ALT_UP_ETHERNET_TRPAL  			0x22	// TX SRAM Read Pointer Address Low Byte
#define ALT_UP_ETHERNET_TRPAH  			0x23	// TX SRAM Read Pointer Address High Byte
#define ALT_UP_ETHERNET_RWPAL  			0x24	// RX SRAM Write Pointer Address Low Byte
#define ALT_UP_ETHERNET_RWPAH  			0x25	// RX SRAM Write Pointer Address High Byte

#define ALT_UP_ETHERNET_VID    			0x28	// Vendor ID
#define ALT_UP_ETHERNET_PID    			0x2A	// Product ID
#define ALT_UP_ETHERNET_CHIPR  			0x2C	// Chip Revision

#define ALT_UP_ETHERNET_TCR2  			0x2D	// TX Control Register 2
#define ALT_UP_ETHERNET_OCR  			0x2E	// Operation Control Register
#define ALT_UP_ETHERNET_SMCR 			0x2F	// Special Mode Control Register
#define ALT_UP_ETHERNET_ETXCSR      	0x30  	// TX early Control Register REG. 30 */
#define ALT_UP_ETHERNET_TCSCR			0x31  	// Transmit Check Sum Control Register
#define ALT_UP_ETHERNET_RCSCSR			0x32  	// Receive Check Sum Control Status Register
#define ALT_UP_ETHERNET_MRCMDX      	0xF0  	// RX FIFO I/O port command READ without address increment (for dummy read a byte) from RX SRAM */
#define ALT_UP_ETHERNET_MRCMD       	0xF2  	// RX FIFO I/O port command READ  from RX SRAM */
#define ALT_UP_ETHERNET_MWCMDX			0xF8  	// TX FIFO I/O port command WRITE into TX FIFO without address increment */
#define ALT_UP_ETHERNET_MWCMD       	0xF8  	// TX FIFO I/O port command WRITE into TX FIFO */
#define ALT_UP_ETHERNET_TXPLL       	0xFC
#define ALT_UP_ETHERNET_TXPLH       	0xFD
#define ALT_UP_ETHERNET_ISR         	0xFE  	// NIC Interrupt Status Register REG. FEH */
#define ALT_UP_ETHERNET_IMR         	0xFF  	// NIC Interrupt Mask   Register REG. FFH */

// Ethernet PHY registers
#define ALT_UP_ETHERNET_PHY_BMCR		0x00	// Basic Mode Control Register
#define ALT_UP_ETHERNET_PHY_BMSR		0x01	// Basic Mode Status Register
#define ALT_UP_ETHERNET_PHY_ID1			0x02
#define ALT_UP_ETHERNET_PHY_ID2			0x03
#define ALT_UP_ETHERNET_PHY_ANAR		0x04	// Auto-negotiation Advertisement Register
#define ALT_UP_ETHERNET_PHY_ANLPAR		0x05	// Auto-negitiation Link Partner Ability Register
#define ALT_UP_ETHERNET_PHY_ANER		0x06	// Auto-negitiation Expansion Register

#define ALT_UP_ETHERNET_PHY_DSCR		0x16	// DAVICON  Specified Configuration Register
#define ALT_UP_ETHERNET_PHY_DSCSR		0x16	// DAVICON  Specified Configuration and Status Register

// Masks
#define ALT_UP_ETHERNET_ISR_PR_MSK		0x01	// Packet Received
#define ALT_UP_ETHERNET_ISR_PT_MSK		0x02	// Packet Transmitted
#define ALT_UP_ETHERNET_ISR_ROS_MSK		0x04	// Receive Overflow
#define ALT_UP_ETHERNET_ISR_ROO_MSK		0x04	// Receive Overflow Counter Overflow
#define ALT_UP_ETHERNET_ISR_LNKCHG_MSK	0x40	// Link Status Change

// temp
#define ALT_UP_ETHERNET_NCR_set         0x00
#define ALT_UP_ETHERNET_TCR_set         0x00
#define ALT_UP_ETHERNET_TX_REQUEST      0x01  /* TCR REG. 02 TXREQ Bit [0] = 1 polling Transmit Request command */
#define ALT_UP_ETHERNET_TCR_long        0x40  /* packet disable TX Jabber Timer */
#define ALT_UP_ETHERNET_RCR_set         0x30  /* skip CRC_packet and skip LONG_packet */
#define ALT_UP_ETHERNET_RX_ENABLE       0x01  /* RCR REG. 05 RXEN Bit [0] = 1 to enable RX machine */
#define ALT_UP_ETHERNET_RCR_long        0x40  /* packet disable RX Watchdog Timer */
#define ALT_UP_ETHERNET_PASS_MULTICAST  0x08  /* RCR REG. 05 PASS_ALL_MULTICAST Bit [3] = 1: RCR_set value ORed 0x08 */
#define ALT_UP_ETHERNET_BPTR_set        0x3F  /* BPTR REG. 08 RX Back Pressure Threshold: High Water Overflow Threshold setting 3KB and Jam_Pattern_Time = 600 us */
#define ALT_UP_ETHERNET_FCTR_set        0x5A  /* FCTR REG. 09 High/ Low Water Overflow Threshold setting 5KB/ 10KB */
#define ALT_UP_ETHERNET_RTFCR_set       0x29  /* RTFCR REG. 0AH RX/TX Flow Control Register enable TXPEN + BKPM(TX_Half) + FLCE(RX) */
#define ALT_UP_ETHERNET_ETXCSR_set      0x83  /* Early Transmit Bit [7] Enable and Threshold 0~3: 12.5%, 25%, 50%, 75% */
#define ALT_UP_ETHERNET_INTR_EN_MASK	0x8F  /* IMR REG. FFH: PAR +PRM, or 0x83: PAR + PRM + PTM */
#define ALT_UP_ETHERNET_PAR_set         0x80  /* IMR REG. FFH: PAR only, RX/TX FIFO R/W Pointer Auto Return enable */

#define MAX_PACKET_SIZE                 1522

#define ALT_UP_ETHERNET_NO_PKT			0x00
#define ALT_UP_ETHERNET_RECEIVED_PKT	0x01

#define ALT_UP_ETHERNET_STATUS_OK		0
#define ALT_UP_ETHERNET_ERROR_UNKNOWN	-1
#define ALT_UP_ETHERNET_ERROR_NO_PKT	-2
#define ALT_UP_ETHERNET_ERROR_CORRUPT	-3

#endif  /* __ALTERA_UP_AVALON_ETHERNET_REGS_H_ */


