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
 * Altera does not recommend, suggest or require that this reference design    *
 * file be used in conjunction or combination with any other product.          *
 *                                                                             *
 ******************************************************************************/

#ifndef __ALTERA_UP_AVALON_USB_PTD_H__
#define __ALTERA_UP_AVALON_USB_PTD_H__

#include "alt_types.h"
#include "altera_up_avalon_usb.h"

struct ptd_struct {
    alt_u16 actualBytes;
    alt_u8 completionCode;
    alt_u8 active;
    alt_u8 toggle;
    alt_u16 maxPacketSize;
    alt_u8 endpointNumber;
    alt_u8 last; // For ISTL (ISO) only
    alt_u8 speed;
    alt_u16 totalBytes;
    alt_u8 paired; // For ATL only
    alt_u8 pingPong; // For ATL only
    alt_u8 dirToken;
    alt_u8 functionAddress;
    alt_u8 pollingRate; // For Interrupt only
    alt_u8 startingFrame; // For ISTL (ISO) and Interrupt
};

// Completion Codes
#define ALT_UP_USB_PTD_CC_NoError			0x0
#define ALT_UP_USB_PTD_CC_CRC				0x1
#define ALT_UP_USB_PTD_CC_BitStuffing			0x2
#define ALT_UP_USB_PTD_CC_DataToggleMismatch            0x3
#define ALT_UP_USB_PTD_CC_Stall                         0x4
#define ALT_UP_USB_PTD_CC_DeviceNotResponding           0x5
#define ALT_UP_USB_PTD_CC_PIDCheckFailure		0x6
#define ALT_UP_USB_PTD_CC_UnexpectedPID			0x7
#define ALT_UP_USB_PTD_CC_DataOverrun			0x8
#define ALT_UP_USB_PTD_CC_DataUnderrun			0x9
#define ALT_UP_USB_PTD_CC_BufferOverrun			0xC
#define ALT_UP_USB_PTD_CC_BufferUnderrun		0xD
#define ALT_UP_USB_PTD_CC_Initial			0xF

void alt_up_usb_init_ptd(struct ptd_struct * ptd, unsigned int dirToken, alt_u8 endpointNumber, alt_u16 packetSize, alt_u8 toggle, alt_u8 functionAddress, alt_u8 portNumber);
//void alt_up_usb_init_ptd (struct ptd_struct * ptd, alt_u16 packetSize, alt_u8 functionAddress, alt_u8 portNumber);
//int alt_up_usb_make_ptd (struct ptd_struct * ptd, alt_u8 dirToken, alt_u8 endpointNumber, alt_u16 maxPacketSize, alt_u8 toggle, alt_u8 functionAddress, alt_u8 portNumber)
void alt_up_usb_convert_ptd_to_array(unsigned int * ptd_array, struct ptd_struct * ptd);

#endif /* __ALTERA_UP_AVALON_USB_PTD_H__ */


