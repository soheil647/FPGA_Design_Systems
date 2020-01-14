module rom(clk, rst, ADR, DATA);

  parameter WIDTH = 16;
  parameter LENGHT = 64;
  
  input clk,rst;
  input [15:0] ADR;
  output [WIDTH - 1:0] DATA;
  
  reg signed [WIDTH:0]coeffs[0:LENGHT];
  
  initial $readmemb("coeffs.txt", coeffs);

  assign DATA = coeffs[ADR];

endmodule
