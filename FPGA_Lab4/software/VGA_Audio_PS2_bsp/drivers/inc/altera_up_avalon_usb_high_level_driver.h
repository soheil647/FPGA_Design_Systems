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

#ifndef __ALTERA_UP_AVALON_USB_HIGH_LEVEL_DRIVER_H__
#define __ALTERA_UP_AVALON_USB_HIGH_LEVEL_DRIVER_H__

#include "alt_types.h"
#include "altera_up_avalon_usb.h"

#define ALT_UP_USB_PORT_1 1
#define ALT_UP_USB_PORT_2 2

#define ALT_UP_USB_PORT_DISCONNECTED 0
#define ALT_UP_USB_PORT_CONNECTED 	 1

#define ALT_UP_USB_FULL_SPEED 0
#define ALT_UP_USB_LOW_SPEED  1

#define ALT_UP_USB_DIR_TOKEN_SETUP 0
#define ALT_UP_USB_DIR_TOKEN_OUT   1
#define ALT_UP_USB_DIR_TOKEN_IN    2


unsigned int alt_up_usb_setup(alt_up_usb_dev * usb_device, int * addr_ptr, int * port_ptr);
int alt_up_usb_reset(alt_up_usb_dev * usb_device);
int alt_up_usb_enable_ports(alt_up_usb_dev * usb_device);

int alt_up_usb_hc_set_operational(alt_up_usb_dev * usb_device);
int alt_up_usb_hc_setup_atl_buffer(alt_up_usb_dev * usb_device);
int alt_up_usb_hc_initialize_defaults(alt_up_usb_dev * usb_device);

void alt_up_usb_hc_disable_all_interrupts(alt_up_usb_dev * usb_device);

void alt_up_usb_hc_set_istl_buffer_size(alt_up_usb_dev * usb_device, unsigned int buffer_size);
void alt_up_usb_hc_set_intl_buffer_size(alt_up_usb_dev * usb_device, unsigned int buffer_size);
void alt_up_usb_hc_set_atl_buffer_size(alt_up_usb_dev * usb_device, unsigned int buffer_size);

int alt_up_usb_get_connection_status(alt_u8 portNumber);
int alt_up_usb_get_port_speed(alt_u8 portNumber);

unsigned int alt_up_usb_send_control(alt_up_usb_dev * usb_device, unsigned int * control_packet, unsigned int * return_packet);
unsigned int alt_up_usb_set_address(alt_up_usb_dev * usb_device, int port, int old_addr, int new_addr);
unsigned int alt_up_usb_set_config(alt_up_usb_dev * usb_device, int addr, int port, int config);

unsigned int alt_up_usb_send_int_packet(alt_up_usb_dev * usb_device, unsigned int * control_packet, unsigned int * return_packet);
unsigned int alt_up_usb_get_control(alt_up_usb_dev * usb_device, unsigned int * return_packet, unsigned int addr, char control_type, unsigned int extra, int port);
unsigned int alt_up_usb_addr_info(int addr, int mode, int dtype, int para);

unsigned int alt_up_usb_assign_address(alt_up_usb_dev * usb_device, unsigned int new_port1_addr, unsigned int new_port2_addr, unsigned int print_port_info);
unsigned int alt_up_usb_hc_buffer_size_info(alt_up_usb_dev * usb_device);

#endif /* __ALTERA_UP_AVALON_USB_HIGH_LEVEL_DRIVER_H__ */


