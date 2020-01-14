module dp
	(
		input [15:0]X_in,
		input clk,rst,clear_k,ce_k,Write,Read,Load,init_reg,ce_n,clear_n,
		output [37:0]out,
		output reg [15:0]out_N
	);
	reg [15:0]Adr_n;
	reg [15:0]Adr_k,cnt_n;
	wire [15:0]Out_Rom;
	wire [15:0]Out_Ram;
	wire [31:0]Out_Multiplier;
	wire [37:0]Out_Adder;

	assign Adr_n = cnt_n - Adr_k;
	assign out_N = Adr_n;

	Counter K 
	(
		.clk(clk),
		.count_up(ce_k),
		.clear   (clear_k),
		.w       (Adr_k),
		.rst     (rst)
	);
	Counter N 
	(
		.clk(clk),
		.count_up(ce_n),
		.clear   (clear_n),
		.w       (cnt_n),
		.rst     (rst)
	);

	// rom ROM
	// (
	// 	.clk (clk),
	// 	.rst (rst),
	// 	.ADR (Adr_k),
	// 	.DATA(Out_Rom)
	// );

	ROOOM rom(
	.address(Adr_k[5:0]),
	.clock(clk),
	.q(Out_Rom)
	);

	Multiplier M(
		
		.INA(Out_Rom),
		.INB(Out_Ram),
		.OUT(Out_Multiplier)
	);
	ADD Adder(

		.INA(out),
		.INB(Out_Multiplier),
		.OUT(Out_Adder)
	);
	memory Memory(

		.clk(clk),
		.write_data(X_in),
		.address   (Adr_n),
		.mem_read  (Read),
		.mem_write (Write),
		.read_data (Out_Ram)
	);
	register Reg(

		.clk(clk),
		.rst(init_reg),
		.d  (Out_Adder),
		.q  (out),
		.ld (Load)
	);


endmodule


