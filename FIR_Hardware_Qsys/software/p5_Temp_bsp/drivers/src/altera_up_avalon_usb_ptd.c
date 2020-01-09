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

#include "alt_types.h"

#include "altera_up_avalon_usb_high_level_driver.h"
#include "altera_up_avalon_usb_ptd.h"

void alt_up_usb_init_ptd(struct ptd_struct * ptd, unsigned int dirToken, alt_u8 endpointNumber, alt_u16 packetSize, alt_u8 toggle, alt_u8 functionAddress, alt_u8 portNumber) {
    ptd->actualBytes = 0;
    ptd->completionCode = ALT_UP_USB_PTD_CC_Initial;
    ptd->active = 1;
    ptd->toggle = toggle;
    ptd->maxPacketSize = packetSize;
    ptd->endpointNumber = endpointNumber;
    ptd->last = 0;
    ptd->speed = alt_up_usb_get_port_speed(portNumber);
    ptd->totalBytes = packetSize;
    ptd->paired = 0;
    ptd->pingPong = 0;
    ptd->dirToken = dirToken; 
    ptd->functionAddress = functionAddress;
    ptd->pollingRate = 0;
    ptd->startingFrame = 0;

    return;
}


// For PTD array format, see ISP1362 porduct datasheet page 42 

void alt_up_usb_convert_ptd_to_array(unsigned int * ptd_array, struct ptd_struct * ptd) {
    ptd_array[0] =
            ((ptd->completionCode & 0xf) << 12) |
            ((ptd->active & 0x1) << 11) |
            ((ptd->toggle & 0x1) << 10) |
            (ptd->actualBytes & 0x3FF);

    ptd_array[1] =
            ((ptd->endpointNumber & 0xF) << 12) |
            ((ptd->last & 0x1) << 11) |
            ((ptd->speed & 0x1) << 10) |
            (ptd->maxPacketSize & 0x3FF);

    ptd_array[2] =
            ((ptd->paired & 0x1) << 15) |
            ((ptd->pingPong & 0x1) << 14) |
            ((ptd->dirToken & 0x3) << 10) |
            (ptd->totalBytes & 0x3FF);

    ptd_array[3] =
            ((ptd->pollingRate & 0x7) << 13) |
            ((ptd->startingFrame & 0xFF) << 8) |
            (ptd->functionAddress & 0x7F);

    return;
}

