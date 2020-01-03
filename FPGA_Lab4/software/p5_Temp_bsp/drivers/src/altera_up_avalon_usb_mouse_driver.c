
#include <stdio.h>

#include "alt_types.h"

#include <altera_up_avalon_usb_regs.h>
#include <altera_up_avalon_usb.h>
#include <altera_up_avalon_usb_low_level_driver.h>
#include <altera_up_avalon_usb_high_level_driver.h>
#include <altera_up_avalon_usb_ptd.h>
#include <altera_up_avalon_usb_mouse_driver.h>

int mouse_addr, mouse_port;

unsigned int toggle = 0;

int alt_up_usb_mouse_setup(alt_up_usb_dev * usb_device, int addr, int port) {

    //int parameters
    unsigned long int_skip = 0xFFFFFFFE;
    unsigned long int_last = 0x00000001;
    unsigned int int_blk_size = 64;

    mouse_addr = addr;
    mouse_port = port;

    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcBufferStatus, 0x00);

    //Setup Int Parameters
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcINTLPTDSkipMap, int_skip);
    alt_up_usb_hc_reg_write_32(usb_device, ALT_UP_USB_HcINTLLastPTD, int_last);
    alt_up_usb_hc_reg_write_16(usb_device, ALT_UP_USB_HcINTLBlkSize, int_blk_size);

    return 0;
}

int alt_up_usb_retrieve_mouse_packet(alt_up_usb_dev * usb_device, struct alt_up_usb_mouse_packet * mouse_packet) {

    struct ptd_struct int_control_packet_ptd;
    unsigned int int_control_packet[128];
    unsigned int int_return_packet[128];
    unsigned int timeout_counter;

    // Check if the USB mouse is still connected 
    if ((alt_up_usb_hc_reg_read_16(usb_device, ALT_UP_USB_HcRhPortStatus2) & 0x01) != 0x01) {
        return ALT_UP_USB_MOUSE_NOT_CONNECTED;
    }

    // Send out DATA IN packet
    alt_up_usb_init_ptd(&int_control_packet_ptd, ALT_UP_USB_DIR_TOKEN_IN, 1, 4, toggle % 2, mouse_addr, mouse_port);
    int_control_packet_ptd.startingFrame = 0x00;
    alt_up_usb_convert_ptd_to_array(int_control_packet, &int_control_packet_ptd);
    timeout_counter = alt_up_usb_send_int_packet(usb_device, int_control_packet, int_return_packet);

    if (timeout_counter != 0) {
        mouse_packet->x_movement = (signed char) (int_return_packet[4] >> 8);
        mouse_packet->y_movement = (signed char) (int_return_packet[5] & 0x00FF);
        mouse_packet->buttons = (signed char) (int_return_packet[4] & 0x000F);
        //printf ("\nNew Mouse Packet:\n");
        //printf ("\tX movement is %d\n", *X);
        //printf ("\tY movement is %d\n", *Y);
        //printf ("\tButtons are %X\n", *B);

        toggle++;

        return ALT_UP_USB_MOUSE_PACKET_OK;
    }

    return ALT_UP_USB_MOUSE_NO_PACKET;
}

unsigned int alt_up_usb_play_mouse(alt_up_usb_dev * usb_device, int addr, int port) {

    printf("ISP1362 USB Mouse Demo.....\n");

    alt_up_usb_mouse_setup(usb_device, addr, port);

    alt_up_usb_mouse_packet usb_mouse_packet;

    usb_mouse_packet.x_movement = 0;
    usb_mouse_packet.y_movement = 0;
    usb_mouse_packet.buttons = 0;

    unsigned int pX = 320, pY = 240;

    do {

        pX = pX + usb_mouse_packet.x_movement;
        pY = pY + usb_mouse_packet.y_movement;

        if (pX > 639) {
            pX = 639;
        }
        if (pX < 0) {
            pX = 0;
        }
        if (pY > 479) {
            pY = 479;
        }
        if (pY < 0) {
            pY = 0;
        }

        alt_up_parallel_port_dev * Green_LEDs_dev;
        alt_up_parallel_port_dev * Red_LEDs_dev;
        alt_up_parallel_port_dev * HEX3_HEX0_dev;

        Green_LEDs_dev = alt_up_parallel_port_open_dev("/dev/Green_LEDs");
        Red_LEDs_dev = alt_up_parallel_port_open_dev("/dev/Red_LEDs");
        HEX3_HEX0_dev = alt_up_parallel_port_open_dev("/dev/HEX3_HEX0");

        alt_up_parallel_port_write_data(Red_LEDs_dev, pX);
        alt_up_parallel_port_write_data(Green_LEDs_dev, pY);

        if ((usb_mouse_packet.buttons & 0x1) == 1) { //left button 
            alt_up_parallel_port_write_data(HEX3_HEX0_dev, 0x3f0000);
        }
        if (((usb_mouse_packet.buttons & 0x2) >> 1) == 1) { //right button
            alt_up_parallel_port_write_data(HEX3_HEX0_dev, 0x3f);
        }
        if (((usb_mouse_packet.buttons & 0x4) >> 2) == 1) { //center button
            alt_up_parallel_port_write_data(HEX3_HEX0_dev, 0x3f00);
        }

        usb_mouse_packet.x_movement = 0;
        usb_mouse_packet.y_movement = 0;
        usb_mouse_packet.buttons = 0;

        // Polling and get the data from the mouse	
    } while (alt_up_usb_retrieve_mouse_packet(usb_device, &usb_mouse_packet) != ALT_UP_USB_MOUSE_NOT_CONNECTED);
    printf("Mouse Not Detected\n");
    return 0;
}

void alt_up_usb_mouse_example(alt_up_usb_dev * usb_device) {

    if (usb_device != NULL) {
        //printf("usb_device->base %08x, please check if this matches the USB's base address in Qsys\n", usb_device->base);
        unsigned int mycode;
        int port = -1;
        int addr = -1;
        int config = -1;
        int HID = -1; //Human Interface Device Descriptor number.

        while (1) {
            port = -1;
            addr = -1;

            // 1. Set up the USB and get the connected port number and its address
            HID = alt_up_usb_setup(usb_device, &addr, &port);

            if (port != -1 && HID == 0x0209) {
                // 2. After confirming that the device is connected is a mouse, the host must choose a config on the device
                config = 1;
                mycode = alt_up_usb_set_config(usb_device, addr, port, config);
                if (mycode == 0) {
                    // 3. Set up and play mouse
                    alt_up_usb_play_mouse(usb_device, addr, port);
                }
            }
        }
    }else {
        printf("Error: could not open USB device\n");
    }

}

/*
unsigned int alt_up_usb_play_keyboard(alt_up_usb_dev * usb_device, int addr, int port) {
        struct ptd_struct int_control_packet_ptd;
        unsigned int int_control_packet[128];
        unsigned int int_return_packet[128];

        //buffer information
        unsigned int atllen,ptllen,intllen;
        unsigned int int_start;

        //int parameters
        unsigned long int_skip = 0xFFFFFFFE;
        unsigned long int_last = 0x00000001;
        unsigned int  int_blk_size = 64;

        unsigned int timecnt;
        unsigned int freq;

        signed char X = 0, Y = 0, B = 0;

        unsigned int pX = 320, pY = 240;
        unsigned int oX = 320, oY = 240;

        unsigned int tog = 0;
	
        freq = 0x00;

        alt_up_usb_hc_reg_write_16( usb_device, ALT_UP_USB_HcBufferStatus, 0x00 );

        //Setup Int Parameters
        alt_up_usb_hc_reg_write_32( usb_device, ALT_UP_USB_HcINTLPTDSkipMap, int_skip );
        alt_up_usb_hc_reg_write_32( usb_device, ALT_UP_USB_HcINTLLastPTD, int_last );
        alt_up_usb_hc_reg_write_16( usb_device, ALT_UP_USB_HcINTLBlkSize, int_blk_size );

        //Setup Buffer
        atllen  = alt_up_usb_hc_reg_read_16( usb_device, ALT_UP_USB_HcATLBufferSize);
        ptllen  = alt_up_usb_hc_reg_read_16( usb_device, ALT_UP_USB_HcISTLBufferSize);
        intllen = alt_up_usb_hc_reg_read_16( usb_device, ALT_UP_USB_HcINTLBufferSize);

        int_start = ptllen + ptllen;

        do {
                // send out DATA IN packet
                alt_up_usb_init_ptd( &int_control_packet_ptd, ALT_UP_USB_DIR_TOKEN_IN, 1, 4, tog % 2, addr, port );
                int_control_packet_ptd.startingFrame = freq;
                alt_up_usb_convert_ptd_to_array( int_control_packet, &int_control_packet_ptd );
                timecnt = alt_up_usb_send_int_packet( usb_device, int_control_packet, int_return_packet);
                if (timecnt != 0) {
                        X = (int_return_packet[4] >> 8);
                        Y = (int_return_packet[5] & 0x00FF);
                        B = (int_return_packet[4] & 0x000F);

                        printf ("\nNew Keyboard Packet:\n");
                        printf ("\tPacket byte #4 is %8X\n", int_return_packet[4]);
                        printf ("\tPacket byte #5 is %8X\n", int_return_packet[4]);
                        printf ("\tPacket byte #6 is %8X\n", int_return_packet[4]);
                        printf ("\tPacket byte #7 is %8X\n", int_return_packet[4]);
                } else {
                        X = 0;
                        Y = 0;
                        B = 0;
                }

        } while ((alt_up_usb_hc_reg_read_16( usb_device, ALT_UP_USB_HcRhPortStatus2 ) & 0x01) == 0x01);
        printf("Keyboard Not Detected\n");
}
 */

