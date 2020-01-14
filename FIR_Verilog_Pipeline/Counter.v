module Counter(input clk,rst,count_up,clear,output reg [15:0]w);

	always @(posedge clk, rst) 
	begin
		if (rst)
			w <= 16'b0;
 		else if(count_up)
 			w <= w+1;
  		else if(clear)
  			w <= 16'b0;
  		else
  			w <= w;
	end
endmodule
