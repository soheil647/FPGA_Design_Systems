#ifndef ALTERA_UP_AVALON_USB_MOUSE_DRIVER_H_
#define ALTERA_UP_AVALON_USB_MOUSE_DRIVER_H_

#include <altera_up_avalon_usb.h>
#include <altera_up_avalon_parallel_port.h>
#define ALT_UP_USB_MOUSE_PACKET_OK      0
#define ALT_UP_USB_MOUSE_NO_PACKET      1
#define ALT_UP_USB_MOUSE_NOT_CONNECTED	2

typedef struct alt_up_usb_mouse_packet {
    signed char x_movement;
    signed char y_movement;
    signed char buttons;
} alt_up_usb_mouse_packet;

int alt_up_usb_mouse_setup(alt_up_usb_dev * usb_device, int addr, int port);
int alt_up_usb_retrieve_mouse_packet(alt_up_usb_dev * usb_device, struct alt_up_usb_mouse_packet * mouse_packet);

unsigned int alt_up_usb_play_mouse(alt_up_usb_dev * usb_device, int addr, int port);
void alt_up_usb_mouse_example(alt_up_usb_dev * usb_device);
#endif /*ALTERA_UP_AVALON_USB_MOUSE_DRIVER_H_*/
