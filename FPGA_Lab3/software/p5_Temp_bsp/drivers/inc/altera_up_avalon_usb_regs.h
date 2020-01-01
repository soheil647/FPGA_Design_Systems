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
 * Register definitions for the ISP1362 USB chip used on the 
 * DE2 Development and Education Board
 */

#include <io.h>

#ifndef __ALTERA_UP_AVALON_USB_REGS_H_
#define __ALTERA_UP_AVALON_USB_REGS_H_

#define ALTERA_UP_AVALON_USB_CHIP_ID					0x3630

/*
 * USB Host Control Data Register
 */
#define ALT_UP_USB_HC_DATA_REG					0
#define IOADDR_ALT_UP_USB_HC_DATA(base)			\
			__IO_CALC_ADDRESS_NATIVE(base, ALT_UP_USB_HC_DATA_REG)      
#define IORD_ALT_UP_USB_HC_DATA(base)			\
			IORD(base, ALT_UP_USB_HC_DATA_REG)
#define IOWR_ALT_UP_USB_HC_DATA(base,data)		\
			IOWR(base, ALT_UP_USB_HC_DATA_REG, data)

/*
 * USB Host Control Command Register
 */
#define ALT_UP_USB_HC_COMMAND_REG				1
#define IOADDR_ALT_UP_USB_HC_COMMAND(base)		\
			__IO_CALC_ADDRESS_NATIVE(base, ALT_UP_USB_HC_COMMAND_REG)      
#define IOWR_ALT_UP_USB_HC_COMMAND(base,data)	\
			IOWR(base, ALT_UP_USB_HC_COMMAND_REG, data)

/*
 * USB Device Control Data Register
 */
#define ALT_UP_USB_DC_DATA_REG					2
#define IOADDR_ALT_UP_USB_DC_DATA(base)			\
			__IO_CALC_ADDRESS_NATIVE(base, ALT_UP_USB_DC_DATA_REG)      
#define IORD_ALT_UP_USB_DC_DATA(base)			\
			IORD(base, ALT_UP_USB_DC_DATA_REG)
#define IOWR_ALT_UP_USB_DC_DATA(base,data)		\
			IOWR(base, ALT_UP_USB_DC_DATA_REG, data)

/*
 * USB Device Control Command Register
 */
#define ALT_UP_USB_DC_COMMAND_REG				3
#define IOADDR_ALT_UP_USB_DC_COMMAND(base)		\
			__IO_CALC_ADDRESS_NATIVE(base, ALT_UP_USB_DC_COMMAND_REG)      
#define IOWR_ALT_UP_USB_DC_COMMAND(base,data)	\
			IOWR(base, ALT_UP_USB_DC_COMMAND_REG, data)


/*
 * USB Host Control Register Addresses
 */
// Control and Status Registers
#define ALT_UP_USB_HcRevision					0x00
#define ALT_UP_USB_HcControl					0x01
#define ALT_UP_USB_HcCommandStatus				0x02
#define ALT_UP_USB_HcInterruptStatus			0x03
#define ALT_UP_USB_HcInterruptEnable			0x04
#define ALT_UP_USB_HcInterruptDisable			0x05
// Frame Counter Registers
#define ALT_UP_USB_HcFmInterval					0x0D
#define ALT_UP_USB_HcFmRemaining				0x0E
#define ALT_UP_USB_HcFmNumber					0x0F
#define ALT_UP_USB_HcLSThreshold				0x11
// Root Hub Registers
#define ALT_UP_USB_HcRhDescriptorA				0x12
#define ALT_UP_USB_HcRhDescriptorB				0x13
#define ALT_UP_USB_HcRhStatus					0x14
#define ALT_UP_USB_HcRhPortStatus1				0x15
#define ALT_UP_USB_HcRhPortStatus2				0x16
// DMA and Interrupt Control Registers
#define ALT_UP_USB_HcHardwareConfiguration		0x20
#define ALT_UP_USB_HcDMAConfiguration			0x21
#define ALT_UP_USB_HcTransferCounter			0x22
#define ALT_UP_USB_HcuPInterrupt				0x24
#define ALT_UP_USB_HcuPInterruptEnable			0x25
// Miscellaneous Registers
#define ALT_UP_USB_HcChipID						0x27
#define ALT_UP_USB_HcScratch					0x28
#define ALT_UP_USB_HcSoftwareReset				0x29
// Buffer RAM Control Registers
#define ALT_UP_USB_HcBufferStatus				0x2C
#define ALT_UP_USB_HcDirectAddressLength		0x32
#define ALT_UP_USB_HcDirectAddressData			0x45
// ISO Transfer Registers
#define ALT_UP_USB_HcISTLBufferSize				0x30
#define ALT_UP_USB_HcISTL0BufferPort			0x40
#define ALT_UP_USB_HcISTL1BufferPort			0x42
#define ALT_UP_USB_HcISTLToggleRate				0x47
// Interrupt Transfer Registers
#define ALT_UP_USB_HcINTLBufferSize				0x33
#define ALT_UP_USB_HcINTLBufferPort				0x43
#define ALT_UP_USB_HcINTLBlkSize				0x53
#define ALT_UP_USB_HcINTLPTDDoneMap				0x17
#define ALT_UP_USB_HcINTLPTDSkipMap				0x18
#define ALT_UP_USB_HcINTLLastPTD				0x19
#define ALT_UP_USB_HcINTLCurrentActivePTD		0x1A
// Aperiodic Transfer Registers
#define ALT_UP_USB_HcATLBufferSize				0x34
#define ALT_UP_USB_HcATLBufferPort				0x44
#define ALT_UP_USB_HcATLBlkSize					0x54
#define ALT_UP_USB_HcATLPTDDoneMap				0x1B
#define ALT_UP_USB_HcATLPTDSkipMap				0x1C
#define ALT_UP_USB_HcATLLastPTD					0x1D
#define ALT_UP_USB_HcATLCurrentActivePTD		0x1E
#define ALT_UP_USB_HcATLPTDDoneThresholdCount	0x51
#define ALT_UP_USB_HcATLPTDDoneThresholdTimeOut	0x52
// OTG Control Registers
#define ALT_UP_USB_OTGControl					0x62
#define ALT_UP_USB_OTGStatus					0x67
#define ALT_UP_USB_OTGInterrupt					0x68
#define ALT_UP_USB_OTGInterruptEnable			0x69
#define ALT_UP_USB_OTGTimer						0x6A
#define ALT_UP_USB_OTGAltTimer					0x6C

#endif  /* __ALTERA_UP_AVALON_USB_REGS_H_ */

