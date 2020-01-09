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
 * This module decodes a NTSC video stream.                                   *
 *                                                                            *
 ******************************************************************************/

module altera_up_video_itu_656_decoder (
	// Inputs
	clk,
	reset,

	TD_DATA,

	ready,

	// Bidirectional

	// Outputs
	data,
	startofpacket,
	endofpacket,
	valid
);

/*****************************************************************************
 *                           Parameter Declarations                          *
 *****************************************************************************/


/*****************************************************************************
 *                             Port Declarations                             *
 *****************************************************************************/

// Inputs
input							clk;
input							reset;

input				[ 7: 0]	TD_DATA;

input							ready;

// Bidirectional

// Outputs
output			[15: 0]	data;
output						startofpacket;
output						endofpacket;
output					 	valid;
//output	reg	[15: 0]	data;
//output	reg				startofpacket;
//output	reg			 	valid;

/*****************************************************************************
 *                           Constant Declarations                           *
 *****************************************************************************/

/*****************************************************************************
 *                 Internal Wires and Registers Declarations                 *
 *****************************************************************************/
// Internal Wires
wire						 	timing_reference; // 4-Bytes: FF 00 00 XY

wire						 	start_of_an_even_line;
wire						 	start_of_an_odd_line;

wire				[ 7: 0]	last_data;

// Internal Registers
reg				[ 7: 0]	io_register;
reg		 		[ 7: 0]	video_shift_reg [ 5: 1];

reg							possible_timing_reference;

reg				[ 6: 1]	active_video;

reg							last_line;

reg				[15: 0]	internal_data;
reg							internal_startofpacket;
reg				 			internal_valid;

// State Machine Registers

// Integers
integer						i;

/*****************************************************************************
 *                         Finite State Machine(s)                           *
 *****************************************************************************/


/*****************************************************************************
 *                             Sequential Logic                              *
 *****************************************************************************/
// Input Registers
always @ (posedge clk)
	io_register	<= TD_DATA;

// Output Registers
/*
always @ (posedge clk)
	data	<= {video_shift_reg[5], video_shift_reg[4]};

always @ (posedge clk)
begin
	if (~last_line & start_of_an_odd_line)
		startofpacket	<= 1'b1;
	else if (last_line & start_of_an_even_line)
		startofpacket	<= 1'b1;
	else if (valid)
		startofpacket	<= 1'b0;
end

always @(posedge clk)
begin
	if (active_video[5])
		valid <= valid ^ 1'b1;
	else
		valid <= 1'b0;
end
*/
// Internal Registers
always @ (posedge clk)
begin
	for (i = 5; i > 1; i = i - 1)
		video_shift_reg[i] <= video_shift_reg[(i - 1)];
	video_shift_reg[1] <= io_register;
end

always @(posedge clk)
begin
	if ((video_shift_reg[3] == 8'hFF) && 
			(video_shift_reg[2] == 8'h00) && 
			(video_shift_reg[1] == 8'h00))
		possible_timing_reference <= 1'b1;
	else
		possible_timing_reference <= 1'b0;
end

always @ (posedge clk)
begin
	if (reset)
		active_video 		<= 6'h00;
	else if (start_of_an_even_line | start_of_an_odd_line)
		active_video 		<= {active_video[5:1], 1'b1};
	else if (timing_reference == 1'b1)
		active_video		<= 6'h00;
	else
		active_video[6:2]	<= active_video[5:1];
end

always @ (posedge clk)
begin
	if (reset)
		last_line <= 1'b0;
	else if (start_of_an_odd_line)
		last_line <= 1'b1;
	else if (start_of_an_even_line)
		last_line <= 1'b0;
end

always @ (posedge clk)
	internal_data				<= {video_shift_reg[5], video_shift_reg[4]};

always @ (posedge clk)
begin
	if (~last_line & start_of_an_odd_line)
		internal_startofpacket	<= 1'b1;
	else if (last_line & start_of_an_even_line)
		internal_startofpacket	<= 1'b1;
	else if (valid)
		internal_startofpacket	<= 1'b0;
end

always @(posedge clk)
begin
	if (active_video[5])
		internal_valid			<= internal_valid ^ 1'b1;
	else
		internal_valid			<= 1'b0;
end

/*****************************************************************************
 *                            Combinational Logic                            *
 *****************************************************************************/
// Output Assignments

// Internal Assignments
assign last_data = video_shift_reg[1];

assign timing_reference = 
	(  possible_timing_reference &
	 ( (last_data[5] ^ last_data[4])				==  last_data[3]) &
	 ( (last_data[6] ^ last_data[4])				==  last_data[2]) &
	 ( (last_data[6] ^ last_data[5])				==  last_data[1]) &
	 ( (last_data[6] ^ last_data[5] ^ last_data[4])	==  last_data[0])
	);
	
assign start_of_an_even_line = timing_reference & 
		last_data[6] & ~last_data[5] & ~last_data[4];

assign start_of_an_odd_line	 = timing_reference & 
		~last_data[6] & ~last_data[5] & ~last_data[4];

/*****************************************************************************
 *                              Internal Modules                             *
 *****************************************************************************/

altera_up_video_decoder_add_endofpacket Add_EndofPacket (
	// Inputs
	.clk								(clk),
	.reset							(reset),

	.stream_in_data				(internal_data),
	.stream_in_startofpacket	(internal_startofpacket),
	.stream_in_endofpacket		(1'b0),
	.stream_in_valid				(internal_valid),

	.stream_out_ready				(ready),
	
	// Bidirectional

	// Outputs
	.stream_in_ready				(),

	.stream_out_data				(data),
	.stream_out_startofpacket	(startofpacket),
	.stream_out_endofpacket		(endofpacket),
	.stream_out_valid				(valid)
);
defparam 
	Add_EndofPacket.DW = 15;


endmodule

