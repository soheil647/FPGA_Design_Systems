module FILTER( FIR_input , clk, rst ,ready ,FIR_output, Done);
  
  parameter LENGTH = 38 ;
  parameter WIDTH = 16 ;

  input signed [WIDTH-1:0]FIR_input;
  input clk, rst ,ready;  
	wire clear,en_k,write,read,ld,init,out_n,clear_N;
	wire signed [WIDTH-1:0]w_N;
	output [LENGTH-1:0]FIR_output;
	output Done;
	
 dp DATAPATH
 (
 	.X_in   (FIR_input),
 	.clk    (clk),
 	.rst    (rst),
 	.clear_k(clear),
 	.ce_k   (en_k),
 	.Write  (write),
 	.Read   (read),
 	.Load   (ld),
 	.out    (FIR_output),
 	.out_N  (w_N),
 	.init_reg(init),
 	.ce_n    (out_n),
 	.clear_n (clear_N)
 );
 CU CONTROLLER
 (
 	.clk  (clk),
 	.rst  (rst),
 	.ready(ready),
 	.done (Done),
 	.cn_K (en_k),
 	.write(write),
 	.read (read),
 	.ld   (ld),
 	.init_reg (init),
 	.N      (w_N),
 	.clear_K(clear),
 	.cn_N   (out_n),
 	.clear_N(clear_N)
 	
 );
 endmodule