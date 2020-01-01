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

#ifndef __ALTERA_UP_AVALON_ETHERNET_H__
#define __ALTERA_UP_AVALON_ETHERNET_H__

#include <stddef.h>
#include <alt_types.h>
#include <sys/alt_dev.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 * Device structure definition. Each instance of the driver uses one
 * of these structures to hold its associated state.
 */

typedef struct alt_up_ethernet_dev {
	/// @brief character mode device structure 
	/// @sa Developing Device Drivers for the HAL in Nios II Software Developer's Handbook
	alt_dev dev;
	/// @brief the base address of the device
	unsigned int base;
	unsigned int irq;
	unsigned int interrupt_mask_register;
	unsigned char mac_addr[6];
} alt_up_ethernet_dev;


//////////////////////////////////////////////////////////////////////////
// HAL system functions
//
 /**
 * @brief Initializes the Ethernet device
 *
 * @param ethernet -- the Ethernet device structure
 *
 **/
void alt_up_ethernet_init(alt_up_ethernet_dev *ethernet);

//////////////////////////////////////////////////////////////////////////
// file-like operation functions








//////////////////////////////////////////////////////////////////////////
// direct operation functions

 /**
 * @brief Opens the Ethernet device according to the device name
 *
 * @param name -- the device name in SOPC Builder ("/dev/ethernet" by default)
 *
 * @return the device structure
 **/
alt_up_ethernet_dev* alt_up_ethernet_open_dev(const char* name);


/*
 * Macros used by alt_sys_init 
 */
#define ALTERA_UP_AVALON_ETHERNET_INSTANCE(name, device)	\
  static alt_up_ethernet_dev device =					\
  {                                                 	\
    {                                               	\
      ALT_LLIST_ENTRY,                              	\
      name##_NAME,                                  	\
      NULL , /* open */									\
      NULL , /* close */								\
      NULL , /* read */									\
      NULL , /* write */								\
      NULL , /* lseek */								\
      NULL , /* fstat */								\
      NULL	 /* ioctl */								\
    },                                              	\
	name##_BASE,										\
	name##_IRQ,											\
	0x80,												\
	{ 0x01, 0x60, 0x6E, 0x11, 0x02, 0x0F  }				\
  }

#define ALTERA_UP_AVALON_ETHERNET_INIT(name, device)	\
  {														\
	alt_up_ethernet_init(&device);						\
	/* make the device available to the system */		\
    alt_dev_reg(&device.dev);							\
  }

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ALTERA_UP_AVALON_ETHERNET_H__ */

