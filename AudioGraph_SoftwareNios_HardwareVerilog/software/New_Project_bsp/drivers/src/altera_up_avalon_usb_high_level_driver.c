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

#include <stdio.h>

#include "alt_types.h"

#include <altera_up_avalon_usb_regs.h>
#include <altera_up_avalon_usb.h>
#include <altera_up_avalon_usb_low_level_driver.h>
#include <altera_up_avalon_usb_high_level_driver.h>
#include <altera_up_avalon_usb_ptd.h>

struct {
    alt_u8 usb_port_1_connected;
    alt_u8 usb_port_1_speed;

    alt_u8 usb_port_2_connected;
    alt_u8 usb_port_2_speed;
} usb_settings;

int alt_up_usb_reset(alt_up_usb_dev * usb_device) {
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcControl, 0x00000600);

    return 0;
}

int alt_up_usb_enable_ports(alt_up_usb_dev * usb_device) {

    // Enable and power on both ports 
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcRhPortStatus1, 0x00000102);
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcRhPortStatus2, 0x00000102);

    // Set global power
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcRhStatus, 0x00010000);

    // The duration HCD has to wait before accessing a powered-on port of the Root Hub
    // is set to 64 ms. Each port is individually powered. Number of donwstream ports is 2.
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcRhDescriptorA, 0x20000102);
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcRhDescriptorB, 0x00000000);

    alt_u32 port_1_status = alt_up_usb_hc_reg_read_32(usb_device, ALT_UP_USB_HcRhPortStatus1);
    usb_settings.usb_port_1_connected = ALT_UP_USB_PORT_DISCONNECTED;
    //	printf ("Port 1 Status = %08x\n", port_1_status);
    if ((port_1_status & 0x00000001) != 0) {
        usb_settings.usb_port_1_connected = ALT_UP_USB_PORT_CONNECTED;
        usb_settings.usb_port_1_speed = ALT_UP_USB_FULL_SPEED;
        if ((port_1_status & 0x00000200) != 0) {
            usb_settings.usb_port_1_speed = ALT_UP_USB_LOW_SPEED;
        }
    }

    alt_u32 port_2_status = alt_up_usb_hc_reg_read_32(usb_device, ALT_UP_USB_HcRhPortStatus2);
    usb_settings.usb_port_2_connected = ALT_UP_USB_PORT_DISCONNECTED;
    //	printf ("Port 2 Status = %08x\n", port_2_status);
    if ((port_2_status & 0x00000001) != 0) {
        usb_settings.usb_port_2_connected = ALT_UP_USB_PORT_CONNECTED;
        usb_settings.usb_port_2_speed = ALT_UP_USB_FULL_SPEED;
        if ((port_2_status & 0x00000200) != 0) {
            usb_settings.usb_port_2_speed = ALT_UP_USB_LOW_SPEED;
        }
    }

    //  printf("Port 1 Connected = %d and Speed = %d\n", usb_settings.usb_port_1_connected, usb_settings.usb_port_1_speed);
    //  printf("Port 2 Connected = %d and Speed = %d\n", usb_settings.usb_port_2_connected, usb_settings.usb_port_2_speed);
    return 0;
}

int alt_up_usb_hc_set_operational(alt_up_usb_dev * usb_device) {
    // Control Setup
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcControl, 0x00000680);

    // Frame Counter Setup
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcFmInterval, 0x27782EDF);

    // Root Hub Setup
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcRhDescriptorA, 0x05000B01);
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcRhDescriptorB, 0x00000000);
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcRhPortStatus1, 0x00010100);
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcRhPortStatus2, 0x00010100);

    // Interrupt Setup

    // Hardware Configuration Setup
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcHardwareConfiguration, 0x302d); // set INT1 to Active HIGH, Level Triggered

    return 0;
}

int alt_up_usb_hc_setup_atl_buffer(alt_up_usb_dev * usb_device) {

    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcATLPTDSkipMap, 0xFFFFFFFE);
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcATLLastPTD, 1);
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcATLBlkSize, 64);
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcATLPTDDoneThresholdCount, 1);
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcATLPTDDoneThresholdTimeOut, 5);

    return 0;
}

int alt_up_usb_hc_initialize_defaults(alt_up_usb_dev * usb_device) {
    // Configure buffers to suitable sizes
    alt_up_usb_hc_set_atl_buffer_size(usb_device, 1536);
    alt_up_usb_hc_set_intl_buffer_size(usb_device, 1024);
    alt_up_usb_hc_set_istl_buffer_size(usb_device, 512);

    // Disable all interrupts
    alt_up_usb_hc_disable_all_interrupts(usb_device);

    // Setup Controls for ATL buffer
    alt_up_usb_hc_setup_atl_buffer(usb_device);

    // Disable processing of all buffers
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcBufferStatus, 0);

    return 0;
}

void alt_up_usb_hc_disable_all_interrupts(alt_up_usb_dev * usb_device) {
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcuPInterruptEnable, 0x0000);
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcInterruptDisable, 0xFFFFFFFF);

    return;
}

void alt_up_usb_hc_set_istl_buffer_size(alt_up_usb_dev * usb_device, unsigned int buffer_size) {
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcISTLBufferSize, buffer_size);
    return;
}

void alt_up_usb_hc_set_intl_buffer_size(alt_up_usb_dev * usb_device, unsigned int buffer_size) {
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcINTLBufferSize, buffer_size);
    return;
}

void alt_up_usb_hc_set_atl_buffer_size(alt_up_usb_dev * usb_device, unsigned int buffer_size) {
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcATLBufferSize, buffer_size);
    return;
}

int alt_up_usb_get_connection_status(alt_u8 portNumber) {
    if (portNumber == ALT_UP_USB_PORT_1)
        return usb_settings.usb_port_1_connected;
    return usb_settings.usb_port_2_connected;
}

int alt_up_usb_get_port_speed(alt_u8 portNumber) {
    if (portNumber == ALT_UP_USB_PORT_1)
        return usb_settings.usb_port_1_speed;
    return usb_settings.usb_port_2_speed;
}

void alt_up_usb_delay_loop(unsigned int delay) {
    unsigned int temp = 0;

    do {
        temp++;
    } while (temp < delay);
}

unsigned int alt_up_usb_send_control(alt_up_usb_dev * usb_device, unsigned int * control_packet, unsigned int * return_packet) {
    unsigned int count = 50000;
    unsigned int active_bit;
    unsigned int HcuPInterrupt;
    unsigned int ccode;
    unsigned int timeout = 9;

    //	printf("Control packet to be sent:\n");
    //	for (count = 0; count < 8; count++) {
    //		printf("%4X\n", *(alt_control_packet + count));
    //	}

    do {
        count = 50000;

        alt_up_usb_hc_write_atl(usb_device, control_packet, 8);
        alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcuPInterrupt, 0x0100);
        alt_up_usb_hc_reg_read_32(usb_device, ALT_UP_USB_HcATLPTDDoneMap);
        alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcBufferStatus, 0x0008);

        do {
            HcuPInterrupt = alt_up_usb_hc_reg_read_16(usb_device, ALT_UP_USB_HcuPInterrupt);
            if ((HcuPInterrupt & 0x0100) != 0) {
                active_bit = 0;
            } else {
                active_bit = 1;
            }

            alt_up_usb_delay_loop(50);
            count--;
        } while ((count != 0) && (active_bit != 0));

        alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcBufferStatus, 0x0000);
        alt_up_usb_hc_read_atl(usb_device, return_packet, 72);

        ccode = ((*return_packet) & (0xF000)) >> 12;

        timeout--;
    } while ((ccode != 0) && (timeout != 0));

    return count;
}

unsigned int alt_up_usb_set_address(alt_up_usb_dev * usb_device, int port, int old_addr, int new_addr) {
    struct ptd_struct alt_control_packet_ptd;
    unsigned int alt_control_packet[128] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0500, new_addr, 0x0000, 0x0000};
    unsigned int alt_return_packet[128];
    unsigned int mycode = 0;
    unsigned int tcnt;

    // Read the done map and enable ATL interrupt
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcuPInterrupt, 0x100);
    alt_up_usb_hc_reg_read_32(usb_device, ALT_UP_USB_HcATLPTDDoneMap);

    // Setup stage
    alt_up_usb_init_ptd(&alt_control_packet_ptd, ALT_UP_USB_DIR_TOKEN_SETUP, 0, 8, 0, old_addr, port);
    alt_up_usb_convert_ptd_to_array(alt_control_packet, &alt_control_packet_ptd);

    tcnt = alt_up_usb_send_control(usb_device, alt_control_packet, alt_return_packet);

    mycode = (*alt_return_packet & 0xF000) >> 12;

    // tcnt = 0 means time out
    if (tcnt == 0) {
        mycode |= 0xF000;
    }

    if (mycode == 0) {
        // Send out DATA IN packet
        alt_up_usb_init_ptd(&alt_control_packet_ptd, ALT_UP_USB_DIR_TOKEN_IN, 0, 0, 1, old_addr, port);
        alt_up_usb_convert_ptd_to_array(alt_control_packet, &alt_control_packet_ptd);

        tcnt = alt_up_usb_send_control(usb_device, alt_control_packet, alt_return_packet);

        mycode = (*alt_return_packet & 0xF000) >> 12;
        if (tcnt == 0) {
            mycode |= 0xF000;
        }
    }

    alt_up_usb_hc_reg_read_32(usb_device, ALT_UP_USB_HcATLPTDDoneMap);

    return mycode;
}

void alt_up_usb_array_app(unsigned int *cbuf, unsigned int *tbuf, int no_of_words) {
    int cnt = 0;

    do {
        *(cbuf + cnt) = *(tbuf + cnt);
        cnt++;
    } while (cnt < no_of_words);
}

unsigned int alt_up_usb_get_control(alt_up_usb_dev * usb_device, unsigned int * return_packet, unsigned int addr, char control_type, unsigned int extra, int port) {
    struct ptd_struct alt_control_packet_ptd;
    unsigned int alt_control_packet[128];
    unsigned int alt_return_packet[128];
    unsigned int cnt = 0, lcnt = 0;
    unsigned int toggle_cnt = 0;
    unsigned int word_size;
    unsigned int DesSize, MaxSize;
    MaxSize = 0;

    unsigned int dev_req[4] = {0x0680, 0x0100, 0x0000, 0x8};
    unsigned int cfg_req[4] = {0x0680, 0x0200, 0x0000, 0x8};
    unsigned int str_req[4] = {0x0680, 0x0300, 0x0000, 0x8};
    unsigned int int_req[4] = {0x0680, 0x0400, 0x0000, 0x8};
    unsigned int end_req[4] = {0x0680, 0x0500, 0x0000, 0x8};
    unsigned int hid_req[4] = {0x0681, 0x2100, 0x0000, 0x8};

    unsigned int ccode = 0;
    unsigned int stage = 1;
    unsigned int tout; //timeout indicator

    //STAGE 1 : Send out first setup packet
    alt_up_usb_init_ptd(&alt_control_packet_ptd, ALT_UP_USB_DIR_TOKEN_SETUP, 0, 8, 0, addr, port);
    alt_up_usb_convert_ptd_to_array(alt_control_packet, &alt_control_packet_ptd);
    if (control_type == 'D') {
        alt_up_usb_array_app(alt_control_packet + 4, dev_req, 4);
    }
    if (control_type == 'C') {
        alt_up_usb_array_app(alt_control_packet + 4, cfg_req, 4);
    }
    if (control_type == 'S') {
        alt_up_usb_array_app(alt_control_packet + 4, str_req, 4);
    }
    if (control_type == 'I') {
        alt_up_usb_array_app(alt_control_packet + 4, int_req, 4);
    }
    if (control_type == 'E') {
        alt_up_usb_array_app(alt_control_packet + 4, end_req, 4);
    }
    if (control_type == 'H') {
        alt_up_usb_array_app(alt_control_packet + 4, hid_req, 4);
    }

    if (control_type == 'S') {
        alt_control_packet[5] = alt_control_packet[5] | extra; //This is for string processing
    }

    tout = alt_up_usb_send_control(usb_device, alt_control_packet, alt_return_packet);
    if (tout == 0) {
        ccode |= 0xF000;
    } //Indicates Timeout in transaction

    if (ccode == 0) {
        toggle_cnt++;
        alt_up_usb_init_ptd(&alt_control_packet_ptd, ALT_UP_USB_DIR_TOKEN_IN, 0, 8, toggle_cnt % 2, addr, port);
        alt_up_usb_convert_ptd_to_array(alt_control_packet, &alt_control_packet_ptd);
        tout = alt_up_usb_send_control(usb_device, alt_control_packet, alt_return_packet);
        ccode |= ((alt_return_packet[0] & 0xF000) >> 12);

        if (ccode == 0x09) { //Descriptor Size is less than 8
            ccode = 0;
        }

        if (tout == 0) {
            ccode |= 0xF000;
        } //Indicates Timeout in transaction

        if (control_type != 'C') {
            DesSize = (alt_return_packet[4] & 0x00FF);
        } else {
            DesSize = alt_return_packet[5];
        }

        if (control_type != 'D') {
            MaxSize = alt_up_usb_addr_info(addr, 'R', 'M', MaxSize);
        } else {
            MaxSize = ((alt_return_packet[7] & 0xFF00) >> 8);
            if (MaxSize < 8) {
                MaxSize = 8;
            }
            alt_up_usb_addr_info(addr, 'W', 'M', MaxSize);
        }

        if (control_type == 'H') {
            DesSize = ((alt_return_packet[7] & 0xFF00) >> 8);
            if (DesSize < 8) {
                DesSize = 8;
            }
        }
    }

    if (ccode == 0) {
        // send out DATA OUT packet
        alt_up_usb_init_ptd(&alt_control_packet_ptd, ALT_UP_USB_DIR_TOKEN_OUT, 0, 0, toggle_cnt % 2, addr, port);
        alt_up_usb_convert_ptd_to_array(alt_control_packet, &alt_control_packet_ptd);
        tout = alt_up_usb_send_control(usb_device, alt_control_packet, alt_return_packet);
        if (tout == 0) {
            ccode |= 0xF000;
        } //Indicates Timeout in transaction

        ccode |= ((alt_return_packet[0] & 0xF000) >> 12);
    }
    //STAGE 1: END

    if (ccode == 0) {
        stage = 2;

        hid_req[1] = 0x2200; //Change HID req into HID report descriptor

        //STAGE 2
        alt_up_usb_init_ptd(&alt_control_packet_ptd, ALT_UP_USB_DIR_TOKEN_SETUP, 0, 8, 0, addr, port);
        alt_up_usb_convert_ptd_to_array(alt_control_packet, &alt_control_packet_ptd);
        if (control_type == 'D') {
            alt_up_usb_array_app(alt_control_packet + 4, dev_req, 4);
        }
        if (control_type == 'C') {
            alt_up_usb_array_app(alt_control_packet + 4, cfg_req, 4);
        }
        if (control_type == 'S') {
            alt_up_usb_array_app(alt_control_packet + 4, str_req, 4);
        }
        if (control_type == 'I') {
            alt_up_usb_array_app(alt_control_packet + 4, int_req, 4);
        }
        if (control_type == 'E') {
            alt_up_usb_array_app(alt_control_packet + 4, end_req, 4);
        }
        if (control_type == 'H') {
            alt_up_usb_array_app(alt_control_packet + 4, hid_req, 4);
        }

        if (control_type == 'S') {
            alt_control_packet[5] = alt_control_packet[5] | extra;
        }

        alt_control_packet[7] = DesSize;
        tout = alt_up_usb_send_control(usb_device, alt_control_packet, alt_return_packet);
        if (tout == 0) {
            ccode |= 0xF000;
        } //Indicates Timeout in transaction

        word_size = ((DesSize + 1) >> 1);

        cnt = 0;

        //send out DATA IN packet
        alt_up_usb_init_ptd(&alt_control_packet_ptd, ALT_UP_USB_DIR_TOKEN_IN, 0, MaxSize, 1, addr, port);
        alt_control_packet_ptd.totalBytes = DesSize;
        alt_up_usb_convert_ptd_to_array(alt_control_packet, &alt_control_packet_ptd);

        tout = alt_up_usb_send_control(usb_device, alt_control_packet, alt_return_packet);
        if (tout == 0) {
            ccode |= 0xF000;
        } //Indicates Timeout in transaction

        ccode |= ((alt_return_packet[0] & 0xF000) >> 12);

        if (ccode == 0) { //Data In is sucessful
            lcnt = 0;
            do {
                //Copy the data located right after the 8 bytes PTD
                *(return_packet + cnt) = alt_return_packet[4 + lcnt];
                cnt++;
                lcnt++;
            } while (lcnt < DesSize);
        }
        //STAGE 2: END
    }

    if (ccode == 0) {
        stage = 3;

        //STAGE 3 :Send out DATA OUT packet
        alt_up_usb_init_ptd(&alt_control_packet_ptd, ALT_UP_USB_DIR_TOKEN_OUT, 0, 0, toggle_cnt % 2, addr, port);
        alt_up_usb_convert_ptd_to_array(alt_control_packet, &alt_control_packet_ptd);
        alt_up_usb_send_control(usb_device, alt_control_packet, alt_return_packet);

        //		make_ptd(cbuf,OUT,0,0,toggle_cnt%2,addr,port);
        //		send_control(cbuf,rbuf);

        ccode = ((alt_return_packet[0] & 0xF000) >> 12);
        //STAGE 3: END
    }

    return ((ccode) | (stage << 8));
    // bit 0-3 indicates the error code
    // bit 8-11 indicates at which stage the error was encountered
    // bit 12-15 is F if time-out, else 0
}

unsigned int alt_up_usb_addr_info(int addr, int mode, int dtype, int para) {
    static unsigned int speed[8];
    static unsigned int maxpac[8];
    static unsigned int iman[8];
    static unsigned int ipro[8];

    unsigned int data2return = 0;

    //Displaying data
    if (mode == 'D') {
        printf("\nAddr %d: Speed[%2d] MPS[%4d] iMan[%2d] iPro[%2d]", addr, speed[addr], maxpac[addr], iman[addr], ipro[addr]);
    }

    //Writing data
    if (mode == 'W') {
        if (dtype == 'S') {
            speed[addr] = para;
        }

        if (dtype == 'M') {
            maxpac[addr] = para;
        }

        if (dtype == 'O') //Manufaturer
        {
            iman[addr] = para;
        }

        if (dtype == 'P') //Product
        {
            ipro[addr] = para;
        }
    }

    //Reading data
    if (mode == 'R') {
        if (dtype == 'S') {
            data2return = speed[addr];
        }

        if (dtype == 'M') {
            data2return = maxpac[addr];
        }

        if (dtype == 'O') {
            data2return = iman[addr];
        }

        if (dtype == 'P') {
            data2return = ipro[addr];
        }
    }

    return (data2return);
}

unsigned int alt_up_usb_set_config(alt_up_usb_dev * usb_device, int addr, int port, int config) {
    struct ptd_struct alt_control_packet_ptd;
    unsigned int alt_control_packet[128] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0900, config, 0x0000, 0x0000};
    unsigned int alt_return_packet[128];
    unsigned int tcnt;
    unsigned int mycode = 0;

    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcuPInterrupt, 0x100);

    alt_up_usb_hc_reg_read_32(usb_device, ALT_UP_USB_HcATLPTDDoneMap);

    alt_up_usb_init_ptd(&alt_control_packet_ptd, ALT_UP_USB_DIR_TOKEN_SETUP, 0, 8, 0, addr, port);
    alt_up_usb_convert_ptd_to_array(alt_control_packet, &alt_control_packet_ptd);
    tcnt = alt_up_usb_send_control(usb_device, alt_control_packet, alt_return_packet);
    mycode = (*alt_return_packet & 0xF000) >> 12;

    if (tcnt == 0) {
        mycode |= 0xF000;
    }

    if (mycode == 0) {
        //send out DATA IN packet
        alt_up_usb_init_ptd(&alt_control_packet_ptd, ALT_UP_USB_DIR_TOKEN_IN, 0, 0, 1, addr, port);
        alt_up_usb_convert_ptd_to_array(alt_control_packet, &alt_control_packet_ptd);
        tcnt = alt_up_usb_send_control(usb_device, alt_control_packet, alt_return_packet);
        mycode = (*alt_return_packet & 0xF000) >> 12;
        if (tcnt == 0) {
            mycode |= 0xF000;
        }
    }

    alt_up_usb_hc_reg_read_32(usb_device, ALT_UP_USB_HcATLPTDDoneMap);

    return mycode;
}

unsigned int alt_up_usb_send_int_packet(alt_up_usb_dev * usb_device, unsigned int * control_packet, unsigned int * return_packet) {
    unsigned int cnt = 50000;
    unsigned int active_bit;
    unsigned int UpInt;

    // unsigned int far *dptr;
    unsigned int *dptr;

    dptr = control_packet;

    alt_up_usb_hc_write_int(usb_device, control_packet, 8); //write 16 bytes

    // Enable the INTL_IRQ and enable the HC to process the INTL buffer 
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcuPInterrupt, 0x0080);
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcBufferStatus, 0x04);

    // Read and clear done map, enables INTL interrupt
    alt_up_usb_hc_reg_read_32(usb_device, ALT_UP_USB_HcINTLPTDDoneMap); 

    do {
        UpInt = alt_up_usb_hc_reg_read_16(usb_device, ALT_UP_USB_HcuPInterrupt);
        if ((UpInt & 0x80) != 0) {
            active_bit = 0;
        } else {
            active_bit = 1;
        }

        alt_up_usb_delay_loop(50);
        cnt--;
    } while ((cnt > 0) && (active_bit != 0));

    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcBufferStatus, 0x00);

    alt_up_usb_hc_read_int(usb_device, return_packet, 72);

    return cnt;
}

unsigned int alt_up_usb_assign_address(alt_up_usb_dev * usb_device, unsigned int new_port1_addr, unsigned int new_port2_addr, unsigned int print_port_info) {
    unsigned long root_hub_port1, root_hub_port2;
    unsigned int status;
    unsigned int ccode = 0;

    // Check which port is connected
    root_hub_port1 = alt_up_usb_hc_reg_read_32(usb_device, ALT_UP_USB_HcRhPortStatus1);
    root_hub_port2 = alt_up_usb_hc_reg_read_32(usb_device, ALT_UP_USB_HcRhPortStatus2);
    //printf("\nroot_hub_port1 = %8lX  root_hub_port2 = %8lX\n",root_hub_port1,root_hub_port2);

    alt_up_usb_enable_ports(usb_device);

    if (print_port_info == 1) {
        printf("\nroot_hub_port1 = %8lX  root_hub_port2 = %8lX", root_hub_port1, root_hub_port2);
        printf("\nport1_speed = %2d", alt_up_usb_get_port_speed(1));
        printf("\nport2_speed = %2d", alt_up_usb_get_port_speed(2));
    }

    if (((root_hub_port1 & 0x01) == 1) && ((root_hub_port2 & 0x01) == 1)) {
        if (print_port_info == 1)printf("\nBoth ports has USB device connected.\n");

        //Resets port 1
        alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcRhPortStatus1, 0x00000010);

        //Resets port 2
        alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcRhPortStatus2, 0x00000010);

        //set address for port1 
        ccode = alt_up_usb_set_address(usb_device, 1, 0, new_port1_addr);
        status = 0x0100 | (ccode << 12);

        alt_up_usb_enable_ports(usb_device);

        //set address for port2 
        ccode = alt_up_usb_set_address(usb_device, 2, 0, new_port2_addr);
        status = 0x0001 | (ccode << 4);
    } else if (((root_hub_port1 & 0x01) == 1) && ((root_hub_port2 & 0x01) == 0)) {
        if (print_port_info == 1)printf("\nPort 1 has USB device connected, assigning address 1...\n");

        //set address for port1 		
        ccode = alt_up_usb_set_address(usb_device, 1, 0, new_port1_addr);
        status = 0x0100 | (ccode << 12);
    } else if (((root_hub_port1 & 0x01) == 0) && ((root_hub_port2 & 0x01) == 1)) {
        if (print_port_info == 1)printf("\nPort 2 has USB device connected, assigning address 2...\n");

        //set address for port2 		
        ccode = alt_up_usb_set_address(usb_device, 2, 0, new_port2_addr);
        status = 0x0001 | (ccode << 4);
    } else {
        if (print_port_info == 1)printf("\nNo device connected to ISP1362, aborting enumeration...\n");
        status = 0x0000;
    }

    return (status);
}

unsigned int alt_up_usb_hc_buffer_size_info(alt_up_usb_dev * usb_device) {
    //buffer information
    unsigned int atllen, ptllen, intllen;
    unsigned int int_start;
    unsigned int atl_start;

    atllen = alt_up_usb_hc_reg_read_16(usb_device, ALT_UP_USB_HcATLBufferSize);
    ptllen = alt_up_usb_hc_reg_read_16(usb_device, ALT_UP_USB_HcISTLBufferSize);
    intllen = alt_up_usb_hc_reg_read_16(usb_device, ALT_UP_USB_HcINTLBufferSize);

    int_start = ptllen + ptllen;
    atl_start = ptllen + ptllen + intllen;
    printf("atllen 0x%08x, ptllen 0x%08x, intllen 0x%08x, int_start 0x%08x, atl_start 0x%08x. \n",
            atllen, ptllen, intllen, int_start, atl_start);
    return 0;
}

unsigned int alt_up_usb_setup(alt_up_usb_dev * usb_device, int * addr_ptr, int * port_ptr) {

    unsigned int rbuf[128];

    unsigned int mycode;
    unsigned int iManufacturer, iProduct;
    unsigned int status;
    unsigned int new_port1_addr, new_port2_addr, print_port_info;
    unsigned int extra, HID;

    // 1 means port1 is connected; 2 means port2 is connected; 
    *port_ptr = -1;
    HID = -1;

    while (1) {

        // Configure and Set up the controls of the ATL buffer	
        alt_up_usb_reset(usb_device);
        alt_up_usb_hc_initialize_defaults(usb_device);

        // Change the HC to operational state and Enable the port
        alt_up_usb_hc_set_operational(usb_device);
        alt_up_usb_enable_ports(usb_device);

        // Suspend the host controller, if the system doesn't need it 
        alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcControl, 0x6c0);
        alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcuPInterrupt, 0x1a9);

        // Assign new addresses for port 1 and port 2, maximum addr number is 7			
        new_port1_addr = 1;
        new_port2_addr = 2;
        print_port_info = 0;

        status = alt_up_usb_assign_address(usb_device, new_port1_addr, new_port2_addr, print_port_info);

        // Enable ALT_IRQ and HC suspended
        alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcuPInterruptEnable, 0x120);

        *port_ptr = -1;
        extra = 0;

        if ((status & 0x0001) != 0) {//port 2 active
            *port_ptr = 2;
            *addr_ptr = new_port2_addr;
        } else if ((status & 0x0100) != 0) {//port 1 active
            *port_ptr = 1;
            *addr_ptr = new_port1_addr;
        }

        if (*port_ptr != -1) {
            // Check port for device
            mycode = alt_up_usb_get_control(usb_device, rbuf, *addr_ptr, 'D', extra, *port_ptr);
            if (mycode == 0x0300) {
                iManufacturer = rbuf[7]&0xFF;
                iProduct = (rbuf[7]&0xFF00) >> 8;
                alt_up_usb_addr_info(*addr_ptr, 'W', 'O', iManufacturer);
                alt_up_usb_addr_info(*addr_ptr, 'W', 'P', iProduct);
                mycode = alt_up_usb_get_control(usb_device, rbuf, *addr_ptr, 'H', alt_up_usb_addr_info(*addr_ptr, 'R', 'P', 0), *port_ptr);

                HID = *(rbuf + 1);
                if (HID == 0x0209) { //it must be 0x0209, if connected device is a mouse 
                    printf("\nMouse Detected...\n");
                } else if (HID == 0x0609) { //it must be 0x0609, if connected device is a keyboard
                    printf("\nKeyboard Detected...\n");
                } else {
                    printf("\nUSB Device with HID 0x%04x Detected...\n", HID);
                }
                return HID;

            }
        }

        {
            volatile int usleep;
            for (usleep = 0; usleep < 20000; usleep++);
        }
    }
}
