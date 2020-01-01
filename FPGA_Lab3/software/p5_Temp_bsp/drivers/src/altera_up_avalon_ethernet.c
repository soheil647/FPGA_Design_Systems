

#include <errno.h>
#include <unistd.h>

#include <priv/alt_file.h>

#include "altera_up_avalon_ethernet.h"
#include "altera_up_avalon_ethernet_regs.h"
#include "altera_up_avalon_ethernet_low_level_driver.h"

void alt_up_ethernet_init(alt_up_ethernet_dev *ethernet)
{
	// initialize the device
	unsigned int base = ethernet->base;
	int i;

	// Power on the internal PHY
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_GPR, 0x00 );

	// Software Reset of the Ethernet chip
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_NCR, 0x01 );
	usleep(10);
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_NCR, 0x00 );
	usleep(1);

	// Initialize the MAC Address
	for (i = 0; i < 6; i++) 
		alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_PAR + i, ethernet->mac_addr[i] );

	/* clear any pending interrupt */
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_ISR,  0x3F );
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_NSR,  0x2C );

	/* set PAR bit, don't enable interrupts (do that with the set_interrupts() function */
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_IMR, 0x80 );

	/* enable RX (Broadcast/ ALL_MULTICAST) */
	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_RCR, 0x39 );

	alt_up_ethernet_reg_write( base, ALT_UP_ETHERNET_ETXCSR, 0x03 );

	return;
}

alt_up_ethernet_dev* alt_up_ethernet_open_dev(const char* name)
{
  // find the device from the device list 
  // (see altera_hal/HAL/inc/priv/alt_file.h 
  // and altera_hal/HAL/src/alt_find_dev.c 
  // for details)
  alt_up_ethernet_dev * dev = (alt_up_ethernet_dev *) alt_find_dev( name, &alt_dev_list );

  return dev;
}

