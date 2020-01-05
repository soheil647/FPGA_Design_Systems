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
******************************************************************************/

#ifndef __ALT_UP_ETHERNET_LOW_LEVEL_DRIVERS_H_
#define __ALT_UP_ETHERNET_LOW_LEVEL_DRIVERS_H_

/**
 * @brief Writes the desired index (reg) to the INDEX register to then read the DATA register
 *
 * @param base -- Base address for the Ethernet device (from SOPC Builder)
 * @param reg -- Index value to write to the index register
 *
 * @return Value read from the data register based on the index value
 **/
unsigned int alt_up_ethernet_reg_read(unsigned int base, unsigned int reg);

/**
 * @brief Writes the desired index (reg) to the INDEX register to then write data to the DATA register
 *
 * @param base -- Base address for the Ethernet device (from SOPC Builder)
 * @param reg -- Index value to write to the index register
 * @param data -- Data to write to the data register based on the index value
 *
 **/
void alt_up_ethernet_reg_write(unsigned int base, unsigned int reg, unsigned int data);

/**
 * @brief Reads PHY data based on PHY register address (reg)
 *
 * @param base -- Base address for the Ethernet device (from SOPC Builder)
 * @param reg -- PHY register address to set into EPAR REG
 *
 * @return PHY data read from EPDR REG
 **/
unsigned int alt_up_ethernet_phy_reg_read (unsigned int base, unsigned int reg);

/**
 * @brief Writes PHY data based on PHY register address (reg)
 *
 * @param base -- Base address for the Ethernet device (from SOPC Builder)
 * @param reg -- PHY register address to set into EPAR REG
 * @param data -- PHY data to write into EPDR REG
 *
 **/
void alt_up_ethernet_phy_reg_write (unsigned int base, unsigned int reg, unsigned int data);

#endif  /* __ALT_UP_ETHERNET_LOW_LEVEL_DRIVERS_H_ */

