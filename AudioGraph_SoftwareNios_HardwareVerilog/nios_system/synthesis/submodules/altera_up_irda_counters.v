/******************************************************************************
 * License Agreement                                                          *
 *                                                                            *
 * Copyright (c) 1991-2013 Altera Corporation, San Jose, California, USA.     *
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
 * This module reads and writes data to the IrDA connectpr on Altera's        *
 *  DE1 and DE2 Development and Education Boards.                             *
 *                                                                            *
 ******************************************************************************/

module altera_up_irda_counters (
	// Inputs
	clk,
	reset,
	
	reset_counters,

	// Bidirectionals

	// Outputs
	baud_clock_rising_edge,
	baud_clock_falling_edge,
	capture_in_bit,
	transmitting_bit,
	all_bits_transmitted
);

/*****************************************************************************
 *                           Parameter Declarations                          *
 *****************************************************************************/

parameter CW							= 9;		// BAUD COUNTER WIDTH
parameter BAUD_TICK_COUNT			= 433;
parameter BAUD_3_16_TICK_COUNT	= 81;
parameter CAPTURE_IN_TICK_COUNT	= 60;
parameter HALF_BAUD_TICK_COUNT	= 216;

parameter TDW							= 11;		// TOTAL DATA WIDTH

/*****************************************************************************
 *                             Port Declarations                             *
 *****************************************************************************/
// Inputs
input						clk;
input						reset;

input						reset_counters;

// Bidirectionals

// Outputs
output reg				baud_clock_rising_edge;
output reg				baud_clock_falling_edge;
output reg				capture_in_bit;
output reg				transmitting_bit;
output reg				all_bits_transmitted;

/*****************************************************************************
 *                           Constant Declarations                           *
 *****************************************************************************/


/*****************************************************************************
 *                 Internal Wires and Registers Declarations                 *
 *****************************************************************************/

// Internal Wires

// Internal Registers
reg		[(CW-1):0]	baud_counter;
reg			[ 3: 0]	bit_counter;

// State Machine Registers
reg						ns_counter;
reg						s_counter;

/*****************************************************************************
 *                         Finite State Machine(s)                           *
 *****************************************************************************/
/*
always @(posedge clk)
begin
	if (reset)
		s_counter <= STATE_0_IDLE;
	else
		s_counter <= ns_counter;
end

always @(*)
begin
	ns_counter = STATE_0_IDLE;
	// Defaults
	ns_counter = STATE_0_IDLE;

    case (s_counter)
	STATE_0_IDLE:
		begin
			if ((transfer_data) && (start_and_stop_en))
				ns_counter = STATE_1_RUNNING;
			else
				ns_counter = STATE_0_IDLE;
		end
	STATE_1_RUNNING:
		begin
			if (change_output_bit_en)
				ns_counter = STATE_0_IDLE;
			else
				ns_counter = STATE_1_RUNNING;
		end
	default:
		begin
			ns_counter = STATE_0_IDLE;
		end
	endcase
end
*/
/*****************************************************************************
 *                             Sequential Logic                              *
 *****************************************************************************/

always @(posedge clk)
begin
	if (reset)
		baud_counter <= {CW{1'b0}};
	else if (reset_counters)
		baud_counter <= {CW{1'b0}};
	else if (baud_counter == BAUD_TICK_COUNT)
		baud_counter <= {CW{1'b0}};
	else
		baud_counter <= baud_counter + 1;
end

always @(posedge clk)
begin
	if (reset)
		baud_clock_rising_edge <= 1'b0;
	else if (baud_counter == BAUD_TICK_COUNT)
		baud_clock_rising_edge <= 1'b1;
	else
		baud_clock_rising_edge <= 1'b0;
end

always @(posedge clk)
begin
	if (reset)
		baud_clock_falling_edge <= 1'b0;
	else if (baud_counter == HALF_BAUD_TICK_COUNT)
		baud_clock_falling_edge <= 1'b1;
	else
		baud_clock_falling_edge <= 1'b0;
end

always @(posedge clk)
begin
	if (reset)
		capture_in_bit <= 1'b0;
	else if (baud_counter == CAPTURE_IN_TICK_COUNT)
		capture_in_bit <= 1'b1;
	else
		capture_in_bit <= 1'b0;
end

always @(posedge clk)
begin
	if (reset)
		transmitting_bit <= 1'b0;
	else if (reset_counters)
		transmitting_bit <= 1'b1;
	else if (bit_counter == TDW)
		transmitting_bit <= 1'b0;
	else if (baud_counter == BAUD_3_16_TICK_COUNT)
		transmitting_bit <= 1'b0;
	else if (baud_clock_rising_edge)
		transmitting_bit <= 1'b1;
end

always @(posedge clk)
begin
	if (reset)
		bit_counter <= 4'h0;
	else if (reset_counters)
		bit_counter <= 4'h0;
	else if (bit_counter == TDW)
		bit_counter <= 4'h0;
	else if (baud_counter == BAUD_TICK_COUNT)
		bit_counter <= bit_counter + 4'h1;
end

always @(posedge clk)
begin
	if (reset)
		all_bits_transmitted <= 1'b0;
	else if (bit_counter == TDW)
		all_bits_transmitted <= 1'b1;
	else
		all_bits_transmitted <= 1'b0;
end

/*****************************************************************************
 *                            Combinational Logic                            *
 *****************************************************************************/


/*****************************************************************************
 *                              Internal Modules                             *
 *****************************************************************************/


endmodule

