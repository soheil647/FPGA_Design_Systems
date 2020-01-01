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
 * This module is a FIFO with same clock for both reads and writes.           *
 *                                                                            *
 ******************************************************************************/

module altera_up_sync_fifo (
	// Inputs
	clk,
	reset,

	write_en,
	write_data,

	read_en,
	
	// Bidirectionals

	// Outputs
	fifo_is_empty,
	fifo_is_full,
	words_used,

	read_data
);

/*****************************************************************************
 *                           Parameter Declarations                          *
 *****************************************************************************/

parameter	DW				= 31;		// Data width
parameter	DATA_DEPTH	= 128;
parameter	AW				= 6;		// Address width

/*****************************************************************************
 *                             Port Declarations                             *
 *****************************************************************************/

// Inputs
input						clk;
input						reset;

input						write_en;
input			[DW: 0]	write_data;

input						read_en;

// Bidirectionals

// Outputs
output					fifo_is_empty;
output					fifo_is_full;
output		[AW: 0]	words_used;

output		[DW: 0]	read_data;

/*****************************************************************************
 *                           Constant Declarations                           *
 *****************************************************************************/

/*****************************************************************************
 *                 Internal Wires and Registers Declarations                 *
 *****************************************************************************/

// Internal Wires

// Internal Registers

// State Machine Registers

/*****************************************************************************
 *                         Finite State Machine(s)                           *
 *****************************************************************************/


/*****************************************************************************
 *                             Sequential Logic                              *
 *****************************************************************************/


/*****************************************************************************
 *                            Combinational Logic                            *
 *****************************************************************************/


/*****************************************************************************
 *                              Internal Modules                             *
 *****************************************************************************/


scfifo	Sync_FIFO (
	// Inputs
	.clock			(clk),
	.sclr				(reset),

	.data				(write_data),
	.wrreq			(write_en),

	.rdreq			(read_en),

	// Bidirectionals

	// Outputs
	.empty			(fifo_is_empty),
	.full				(fifo_is_full),
	.usedw			(words_used),
	
	.q					(read_data),

	// Unused
	// synopsys translate_off
	
	.aclr				(),
	.almost_empty	(),
	.almost_full	()
	// synopsys translate_on
);
defparam
	Sync_FIFO.add_ram_output_register	= "OFF",
	Sync_FIFO.intended_device_family		= "Cyclone II",
	Sync_FIFO.lpm_numwords					= DATA_DEPTH,
	Sync_FIFO.lpm_showahead					= "ON",
	Sync_FIFO.lpm_type						= "scfifo",
	Sync_FIFO.lpm_width						= DW + 1,
	Sync_FIFO.lpm_widthu						= AW + 1,
	Sync_FIFO.overflow_checking			= "OFF",
	Sync_FIFO.underflow_checking			= "OFF",
	Sync_FIFO.use_eab							= "ON";

endmodule

