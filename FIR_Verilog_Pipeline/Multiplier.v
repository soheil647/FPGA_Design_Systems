module Multiplier(INA,INB,OUT);
  parameter WIDTH_IN = 16;
  parameter WIDTH_OUT = 32;
  
  input signed [WIDTH_IN - 1:0] INA;
  input signed [WIDTH_IN - 1:0] INB;
  output signed [WIDTH_OUT - 1:0] OUT;
  
	assign OUT = INA*INB;
	
endmodule

