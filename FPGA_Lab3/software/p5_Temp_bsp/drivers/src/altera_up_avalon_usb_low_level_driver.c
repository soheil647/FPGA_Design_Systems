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

#include <altera_up_avalon_usb_regs.h>
#include <altera_up_avalon_usb.h>
#include <altera_up_avalon_usb_low_level_driver.h>

/*
 * Host Controller Function
 */

void alt_up_usb_hc_reg_write_16(alt_up_usb_dev * usb_device, unsigned char reg, unsigned int value) {
    IOWR_ALT_UP_USB_HC_COMMAND(usb_device->base, reg | 0x80);
    IOWR_ALT_UP_USB_HC_DATA(usb_device->base, value);

    return;
}

void alt_up_usb_hc_reg_write_32(alt_up_usb_dev * usb_device, unsigned char reg, unsigned long value) {
    unsigned int low_word;
    unsigned int hi_word;

    low_word = (value)&0x0000FFFF;
    hi_word = ((value)&0xFFFF0000) >> 16;

    IOWR_ALT_UP_USB_HC_COMMAND(usb_device->base, reg | 0x80);
    IOWR_ALT_UP_USB_HC_DATA(usb_device->base, low_word);
    IOWR_ALT_UP_USB_HC_DATA(usb_device->base, hi_word);

    return;
}

unsigned int alt_up_usb_hc_reg_read_16(alt_up_usb_dev * usb_device, unsigned char reg) {
    unsigned int result;

    IOWR_ALT_UP_USB_HC_COMMAND(usb_device->base, reg);
    result = IORD_ALT_UP_USB_HC_DATA(usb_device->base);

    return result;
}

unsigned long alt_up_usb_hc_reg_read_32(alt_up_usb_dev * usb_device, unsigned char reg) {
    unsigned int result_l, result_h;
    unsigned long result;

    IOWR_ALT_UP_USB_HC_COMMAND(usb_device->base, reg);
    result_l = IORD_ALT_UP_USB_HC_DATA(usb_device->base);
    result_h = IORD_ALT_UP_USB_HC_DATA(usb_device->base);

    result = result_h;
    result = result << 16;
    result = result + result_l;

    return (result);
}

void alt_up_usb_hc_write_atl(alt_up_usb_dev * usb_device, unsigned int *a_ptr, unsigned int data_size) {
    int count = 0;

    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcTransferCounter, data_size * 2);
    IOWR_ALT_UP_USB_HC_COMMAND(usb_device->base, ALT_UP_USB_HcATLBufferPort | 0x80);

    do {
        IOWR_ALT_UP_USB_HC_DATA(usb_device->base, *(a_ptr + count));
    } while (++count < data_size);

    return;
}

void alt_up_usb_hc_read_atl(alt_up_usb_dev * usb_device, unsigned int *a_ptr, unsigned int data_size) {
    int count = 0;

    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcTransferCounter, data_size * 2);
    IOWR_ALT_UP_USB_HC_COMMAND(usb_device->base, ALT_UP_USB_HcATLBufferPort);

    do {
        *(a_ptr + count) = IORD_ALT_UP_USB_HC_DATA(usb_device->base);
    } while (++count < data_size);

    return;
}

void alt_up_usb_hc_write_int(alt_up_usb_dev * usb_device, unsigned int *a_ptr, unsigned int data_size) {
    int count = 0;

    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcTransferCounter, data_size * 2);
    IOWR_ALT_UP_USB_HC_COMMAND(usb_device->base, ALT_UP_USB_HcINTLBufferPort | 0x80);

    do {
        IOWR_ALT_UP_USB_HC_DATA(usb_device->base, *(a_ptr + count));
    } while (++count < data_size);

    return;
}

void alt_up_usb_hc_read_int(alt_up_usb_dev * usb_device, unsigned int *a_ptr, unsigned int data_size) {
    int count = 0;

    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcTransferCounter, data_size * 2);
    IOWR_ALT_UP_USB_HC_COMMAND(usb_device->base, ALT_UP_USB_HcINTLBufferPort);

    do {
        *(a_ptr + count) = IORD_ALT_UP_USB_HC_DATA(usb_device->base);
    } while (++count < data_size);

    return;
}

void alt_up_usb_hc_direct_address_write(alt_up_usb_dev * usb_device, unsigned int *a_ptr, unsigned int start_addr, unsigned int data_size) {
    int count = 0;
    unsigned long HcDirectAddrLen_value;
    HcDirectAddrLen_value = (unsigned long) (start_addr & 0x7fff);
    HcDirectAddrLen_value |= (unsigned long) (data_size << 16);

    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcDirectAddressLength, HcDirectAddrLen_value);
    IOWR_ALT_UP_USB_HC_COMMAND(usb_device->base, ALT_UP_USB_HcDirectAddressData | 0x80);

    do {
        IOWR_ALT_UP_USB_HC_DATA(usb_device->base, *(a_ptr + count));
    } while (++count < data_size);

    return;
}

void alt_up_usb_hc_direct_address_read(alt_up_usb_dev * usb_device, unsigned int *a_ptr, unsigned int start_addr, unsigned int data_size) {
    int count = 0;
    unsigned long HcDirectAddrLen_value;
    HcDirectAddrLen_value = (unsigned long) (start_addr & 0x7fff);
    HcDirectAddrLen_value |= (unsigned long) (data_size << 16);

    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcDirectAddressLength, HcDirectAddrLen_value);
    IOWR_ALT_UP_USB_HC_COMMAND(usb_device->base, ALT_UP_USB_HcDirectAddressData);

    do {
        *(a_ptr + count) = IORD_ALT_UP_USB_HC_DATA(usb_device->base);
    } while (++count < data_size);

    return;
}

/*
 * Device Controller Function
 */

void alt_up_usb_dc_reg_write_16(alt_up_usb_dev * usb_device, unsigned char reg, unsigned int value) {
    IOWR_ALT_UP_USB_DC_COMMAND(usb_device->base, reg | 0x80);
    IOWR_ALT_UP_USB_DC_DATA(usb_device->base, value);

    return;
}

void alt_up_usb_dc_reg_write_32(alt_up_usb_dev * usb_device, unsigned char reg, unsigned long value) {
    unsigned int low_word;
    unsigned int hi_word;

    low_word = (value)&0x0000FFFF;
    hi_word = ((value)&0xFFFF0000) >> 16;

    IOWR_ALT_UP_USB_DC_COMMAND(usb_device->base, reg | 0x80);
    IOWR_ALT_UP_USB_DC_DATA(usb_device->base, low_word);
    IOWR_ALT_UP_USB_DC_DATA(usb_device->base, hi_word);

    return;
}

unsigned int alt_up_usb_dc_reg_read_16(alt_up_usb_dev * usb_device, unsigned char reg) {
    unsigned int result;

    IOWR_ALT_UP_USB_DC_COMMAND(usb_device->base, reg);
    result = IORD_ALT_UP_USB_DC_DATA(usb_device->base);

    return result;
}

unsigned long alt_up_usb_dc_reg_read_32(alt_up_usb_dev * usb_device, unsigned char reg) {
    unsigned int result_l, result_h;
    unsigned long result;

    IOWR_ALT_UP_USB_DC_COMMAND(usb_device->base, reg);
    result_l = IORD_ALT_UP_USB_DC_DATA(usb_device->base);
    result_h = IORD_ALT_UP_USB_DC_DATA(usb_device->base);

    result = result_h;
    result = result << 16;
    result = result + result_l;

    return (result);
}





/*
int usb_register_port_1_write (unsigned int usb_device->base, unsigned int reg, unsigned int value)
{
    IOWR_ALTERA_UP_AVALON_ETHERNET_PORT_1_INDEX(usb_device->base, reg);
    IOWR_ALTERA_UP_AVALON_ETHERNET_PORT_1_DATA (usb_device->base, value);
    return 0;
}

int usb_register_read  (unsigned int usb_device->base, unsigned int reg)
{
    return 0;
}

int usb_phy_write (alt_up_usb_dev * usb_device, unsigned char reg, unsigned int value);
int usb_phy_write (unsigned int usb_device->base, unsigned int reg, unsigned int value)
{
    return 0;
}
 */

