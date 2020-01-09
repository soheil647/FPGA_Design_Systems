

module TestTransmit();


reg clk,TxD_start,Rst;
reg[7:0] TxD_data;

wire TxD,TxD_busy;

async_transmitter ts(
    .clk(clk),
    .TxD_start(TxD_start),
    .TxD_data(TxD_data),
    .Rst(Rst),
    .TxD(TxD),
    .TxD_busy(TxD_busy)
);

initial repeat (10000) #20 clk=~clk;

initial begin
  Rst=1'b1;
  clk=1'b0;
  TxD_start=1'b0;
  #40
  Rst=1'b0;
  #200
  TxD_start=1'b1;
  TxD_data=8'd325;
  #200
  TxD_start=1'b0;
  #1000000
  $stop;
end

endmodule
