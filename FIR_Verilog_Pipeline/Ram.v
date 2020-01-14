module memory(input [15:0]write_data,input [15:0]address,input clk,mem_read,mem_write,output [15:0]read_data);
    
reg signed [15:0] reg_mem [0:221184];

always @(posedge clk) begin

	if(mem_write)
		reg_mem[address] <= write_data; 
end
  assign read_data = mem_read ? reg_mem[address] : 16'b0;

endmodule


