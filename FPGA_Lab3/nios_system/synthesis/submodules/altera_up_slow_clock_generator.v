/******************************************************************************
 * License Agreement                                                          *
 *                                                                            *
 * Copyright (c) 1991-2012 Altera Corporation, San Jose, California, USA.     *
 * All rights reserved.                                                       *
 *                                                                            *
 * Any megafunction design, and related net list (encrypted or decrypted),    *
 *  support information, device programming or simulation file, and any other *
 *  associated documentation or information provided by Altera or a partner   *
 *  under Altera's Megafunction Partnership Program may be used only to       *
 *  program PLD devices (but not masked PLD devices) from Altera.  Any other  *
 *  use of such megafunction design, net list, support information, device    *
 *  programming or simulation file, or any other related documentation or     *
 *  information is prohibited for any other purpose, including, but not       *
 *  limited to modification, reverse engineering, de-compiling, or use with   *
 *  any other silicon devices, unless such use is explicitly licensed under   *
 *  a separate agreement with Altera or a megafunction partner.  Title to     *
 *  the intellectual property, including patents, copyrights, trademarks,     *
 *  trade secrets, or maskworks, embodied in any such megafunction design,    *
 *  net list, support information, device programming or simulation file, or  *
 *  any other related documentation or information provided by Altera or a    *
 *  megafunction partner, remains with Altera, the megafunction partner, or   *
 *  their respective licensors.  No other licenses, including any licenses    *
 *  needed under any third party's intellectual property, are provided herein.*
 *  Copying or modifying any file, or portion thereof, to which this notice   *
 *  is attached violates this copyright.                                      *
 *                                                                            *
 * THIS FILE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    *
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    *
 * FROM, OUT OF OR IN CONNECTION WITH THIS FILE OR THE USE OR OTHER DEALINGS  *
 * IN THIS FILE.                                                              *
 *                                                                            *
 * This agreement shall be governed in all respects by the laws of the State  *
 *  of California and by the laws of the United States of America.            *
 *                                                                            *
 ******************************************************************************/

/******************************************************************************
 *                                                                            *
 * This module can create clock signals that have a frequency lower           *
 *  than those a PLL can generate.                                            *
 *                                                                            *
 * Revision: 1.2                                                              *
 *                                                                            *
 * Used in IP Cores:                                                          *
 *   Altera UP Avalon Audio and Video Config                                  *
 *                                                                            *
 ******************************************************************************/

module altera_up_slow_clock_generator (
	// Inputs
	clk,
	reset,
	
	enable_clk,

	// Bidirectionals

	// Outputs
	new_clk,

	rising_edge,
	falling_edge,

	middle_of_high_level,
	middle_of_low_level
);

/*****************************************************************************
 *                           Parameter Declarations                          *
 *****************************************************************************/

parameter CB				= 10;			//COUNTER BITS

/*****************************************************************************
 *                             Port Declarations                             *
 *****************************************************************************/

// Inputs
input						clk;
input						reset;

input						enable_clk;
	
// Bidirectionals

// Outputs
output reg				new_clk;

output reg				rising_edge;
output reg				falling_edge;

output reg				middle_of_high_level;
output reg				middle_of_low_level;

/*****************************************************************************
 *                           Constant Declarations                           *
 *****************************************************************************/

/*****************************************************************************
 *                 Internal Wires and Registers Declarations                 *
 *****************************************************************************/

// Internal Wires

// Internal Registers
reg			[CB:1]	clk_counter;

// State Machine Registers

/*****************************************************************************
 *                         Finite State Machine(s)                           *
 *****************************************************************************/


/*****************************************************************************
 *                             Sequential Logic                              *
 *****************************************************************************/

always @(posedge clk)
begin
	if (reset)
		clk_counter	<= 'h0;
	else if (enable_clk)
		clk_counter	<= clk_counter + 1;
end

always @(posedge clk)
begin
	if (reset)
		new_clk	<= 1'b0;
	else
		new_clk	<= clk_counter[CB];
end

always @(posedge clk)
begin
	if (reset)
		rising_edge	<= 1'b0;
	else
		rising_edge	<= (clk_counter[CB] ^ new_clk) & ~new_clk;
end

always @(posedge clk)
begin
	if (reset)
		falling_edge <= 1'b0;
	else
		falling_edge <= (clk_counter[CB] ^ new_clk) & new_clk;
end

always @(posedge clk)
begin
	if (reset)
		middle_of_high_level <= 1'b0;
	else
		middle_of_high_level <= 
			clk_counter[CB] & 
			~clk_counter[(CB - 1)] &
			(&(clk_counter[(CB - 2):1]));
end

always @(posedge clk)
begin
	if (reset)
		middle_of_low_level <= 1'b0;
	else
		middle_of_low_level <= 
			~clk_counter[CB] & 
			~clk_counter[(CB - 1)] &
			(&(clk_counter[(CB - 2):1]));
end



/*****************************************************************************
 *                            Combinational Logic                            *
 *****************************************************************************/

// Output Assignments

// Internal Assignments

/*****************************************************************************
 *                              Internal Modules                             *
 *****************************************************************************/

endmodule

