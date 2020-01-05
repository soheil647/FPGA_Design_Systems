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
 * This module store and retrieves video frames to and from memory.           *
 *                                                                            *
 ******************************************************************************/

`undef USE_TO_MEMORY
`define USE_32BIT_MASTER

module nios_system_Video_In_DMA_Controller (
	// Inputs
	clk,
	reset,

	stream_data,
	stream_startofpacket,
	stream_endofpacket,
	stream_empty,
	stream_valid,

	master_waitrequest,
	
	slave_address,
	slave_byteenable,
	slave_read,
	slave_write,
	slave_writedata,

	// Bidirectional

	// Outputs
	stream_ready,

	master_address,
	master_write,
	master_writedata,

	slave_readdata
);

/*****************************************************************************
 *                           Parameter Declarations                          *
 *****************************************************************************/

parameter DW								= 15; // Frame's datawidth
parameter EW								= 0; // Frame's empty width
parameter WIDTH							= 320; // Frame's width in pixels
parameter HEIGHT							= 240; // Frame's height in lines

parameter AW								= 16; // Frame's address width
parameter WW								= 8; // Frame width's address width
parameter HW								= 7; // Frame height's address width

parameter MDW								= 15; // Avalon master's datawidth

parameter DEFAULT_BUFFER_ADDRESS		= 32'd134217728;
parameter DEFAULT_BACK_BUF_ADDRESS	= 32'd134217728;

parameter ADDRESSING_BITS				= 16'd2057;
parameter COLOR_BITS						= 4'd15;
parameter COLOR_PLANES					= 2'd0;

parameter DEFAULT_DMA_ENABLED			= 1'b0; // 0: OFF or 1: ON

/*****************************************************************************
 *                             Port Declarations                             *
 *****************************************************************************/

// Inputs
input						clk;
input						reset;

input			[DW: 0]	stream_data;
input						stream_startofpacket;
input						stream_endofpacket;
input			[EW: 0]	stream_empty;
input						stream_valid;

input						master_waitrequest;
	
input			[ 1: 0]	slave_address;
input			[ 3: 0]	slave_byteenable;
input						slave_read;
input						slave_write;
input			[31: 0]	slave_writedata;

// Bidirectional

// Outputs
output					stream_ready;

output		[31: 0]	master_address;
output					master_write;
output		[MDW:0]	master_writedata;

output		[31: 0]	slave_readdata;

/*****************************************************************************
 *                           Constant Declarations                           *
 *****************************************************************************/


/*****************************************************************************
 *                 Internal Wires and Registers Declarations                 *
 *****************************************************************************/

// Internal Wires
wire						inc_address;
wire						reset_address;

wire			[31: 0]	buffer_start_address;
wire						dma_enabled;

// Internal Registers
reg			[WW: 0]	w_address;		// Frame's width address
reg			[HW: 0]	h_address;		// Frame's height address

// State Machine Registers

// Integers

/*****************************************************************************
 *                         Finite State Machine(s)                           *
 *****************************************************************************/


/*****************************************************************************
 *                             Sequential Logic                              *
 *****************************************************************************/

// Output Registers

// Internal Registers
always @(posedge clk)
begin
	if (reset)
	begin
		w_address 	<= 'h0;
		h_address 	<= 'h0;
	end
	else if (reset_address)
	begin
		w_address 	<= 'h0;
		h_address 	<= 'h0;
	end
	else if (inc_address)
	begin
		if (w_address == (WIDTH - 1))
		begin
			w_address 	<= 'h0;
			h_address	<= h_address + 1;
		end
		else
			w_address 	<= w_address + 1;
	end
end

/*****************************************************************************
 *                            Combinational Logic                            *
 *****************************************************************************/

// Output Assignments
assign master_address		= buffer_start_address +
								{h_address, w_address, 1'b0};

// Internal Assignments

/*****************************************************************************
 *                              Internal Modules                             *
 *****************************************************************************/

altera_up_video_dma_control_slave DMA_Control_Slave (
	// Inputs
	.clk									(clk),
	.reset								(reset),

	.address								(slave_address),
	.byteenable							(slave_byteenable),
	.read									(slave_read),
	.write								(slave_write),
	.writedata							(slave_writedata),

	.swap_addresses_enable			(reset_address),

	// Bi-Directional

	// Outputs
	.readdata							(slave_readdata),

	.current_start_address			(buffer_start_address),
	.dma_enabled						(dma_enabled)
);
defparam
	DMA_Control_Slave.DEFAULT_BUFFER_ADDRESS		= DEFAULT_BUFFER_ADDRESS,
	DMA_Control_Slave.DEFAULT_BACK_BUF_ADDRESS	= DEFAULT_BACK_BUF_ADDRESS,

	DMA_Control_Slave.WIDTH								= WIDTH,
	DMA_Control_Slave.HEIGHT							= HEIGHT,

	DMA_Control_Slave.ADDRESSING_BITS				= ADDRESSING_BITS,
	DMA_Control_Slave.COLOR_BITS						= COLOR_BITS,
	DMA_Control_Slave.COLOR_PLANES					= COLOR_PLANES,
	DMA_Control_Slave.ADDRESSING_MODE				= 1'b0,

	DMA_Control_Slave.DEFAULT_DMA_ENABLED			= DEFAULT_DMA_ENABLED;

altera_up_video_dma_to_memory From_Stream_to_Memory (
	// Inputs
	.clk									(clk),
	.reset								(reset | ~dma_enabled),

	.stream_data						(stream_data),
	.stream_startofpacket			(stream_startofpacket),
	.stream_endofpacket				(stream_endofpacket),
	.stream_empty						(stream_empty),
	.stream_valid						(stream_valid),

	.master_waitrequest				(master_waitrequest),
	
	// Bidirectional

	// Outputs
	.stream_ready						(stream_ready),

	.master_write						(master_write),
	.master_writedata					(master_writedata),

	.inc_address						(inc_address),
	.reset_address						(reset_address)
);
defparam
	From_Stream_to_Memory.DW	= DW,
	From_Stream_to_Memory.EW	= EW,
	From_Stream_to_Memory.MDW	= MDW;

endmodule

