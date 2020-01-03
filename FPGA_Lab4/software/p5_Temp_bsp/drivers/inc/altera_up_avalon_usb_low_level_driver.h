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

#ifndef __ALTERA_UP_AVALON_USB_LOW_LEVEL_DRIVER_H__
#define __ALTERA_UP_AVALON_USB_LOW_LEVEL_DRIVER_H__

/*
 * Host Controller Function
 */

void alt_up_usb_hc_reg_write_16(alt_up_usb_dev * usb_device, unsigned char reg, unsigned int value);
void alt_up_usb_hc_reg_write_32(alt_up_usb_dev * usb_device, unsigned char reg, unsigned long value);

unsigned int alt_up_usb_hc_reg_read_16(alt_up_usb_dev * usb_device, unsigned char reg);
unsigned long alt_up_usb_hc_reg_read_32(alt_up_usb_dev * usb_device, unsigned char reg);

void alt_up_usb_hc_write_atl(alt_up_usb_dev * usb_device, unsigned int * a_ptr, unsigned int data_size);
void alt_up_usb_hc_read_atl(alt_up_usb_dev * usb_device, unsigned int * a_ptr, unsigned int data_size);

void alt_up_usb_hc_write_int(alt_up_usb_dev * usb_device, unsigned int * a_ptr, unsigned int data_size);
void alt_up_usb_hc_read_int(alt_up_usb_dev * usb_device, unsigned int * a_ptr, unsigned int data_size);

void alt_up_usb_hc_direct_address_write(alt_up_usb_dev * usb_device, unsigned int *a_ptr, unsigned int start_addr, unsigned int data_size);
void alt_up_usb_hc_direct_address_read(alt_up_usb_dev * usb_device, unsigned int *a_ptr, unsigned int start_addr, unsigned int data_size);

/*
 * Device Controller Function
 */

void alt_up_usb_dc_reg_write_16(alt_up_usb_dev * usb_device, unsigned char reg, unsigned int value);
void alt_up_usb_dc_reg_write_32(alt_up_usb_dev * usb_device, unsigned char reg, unsigned long value);

unsigned int alt_up_usb_dc_reg_read_16(alt_up_usb_dev * usb_device, unsigned char reg);
unsigned long alt_up_usb_dc_reg_read_32(alt_up_usb_dev * usb_device, unsigned char reg);

// int alt_up_usb_phy_write(alt_up_usb_dev * usb_device, unsigned char reg, unsigned int value);

#endif /* __ALTERA_UP_AVALON_USB_LOW_LEVEL_DRIVER_H__ */
