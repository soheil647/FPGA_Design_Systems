module ADD(INA,INB,OUT);
  
  parameter WIDTH = 38;
  
  input signed [37:0] INA;
  input signed [31:0] INB;
  output signed [WIDTH - 1:0] OUT;
  
	assign OUT = INA+INB;
	
endmodule