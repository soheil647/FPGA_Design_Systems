module register(input signed[37:0]d,input clk,rst,ld,output reg [37:0]q);

always @(posedge clk) begin

if(rst)
 q <=38'b0;
 else if(ld)
  q <=d;
 end
 endmodule