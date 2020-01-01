module AVM_AVALONMASTER_MAGNITUDE #
(
  // you can add parameters here
  // you can change these parameters
  parameter integer AVM_AVALONMASTER_DATA_WIDTH = 32,
  parameter integer AVM_AVALONMASTER_ADDRESS_WIDTH = 32
)
(
  // user ports begin

  // these are just some example ports. you can change them all
  input wire START,
  output wire DONE,

  // user ports end
  // dont change these ports
  input wire CSI_CLOCK_CLK,
  input wire CSI_CLOCK_RESET,
  output wire [AVM_AVALONMASTER_ADDRESS_WIDTH - 1:0] AVM_AVALONMASTER_ADDRESS,
  input wire AVM_AVALONMASTER_WAITREQUEST,
  output wire AVM_AVALONMASTER_READ,
  output wire AVM_AVALONMASTER_WRITE,
  input wire [AVM_AVALONMASTER_DATA_WIDTH - 1:0] AVM_AVALONMASTER_READDATA,
  output wire [AVM_AVALONMASTER_DATA_WIDTH - 1:0] AVM_AVALONMASTER_WRITEDATA
);

  // output wires and registers
  // you can change name and type of these ports
  reg done;

  reg [AVM_AVALONMASTER_ADDRESS_WIDTH - 1:0] address;
  reg read;
  reg write;
  reg [AVM_AVALONMASTER_DATA_WIDTH - 1:0] writedata;

  // I/O assignment
  // never directly send values to output
  assign DONE = done;
  assign AVM_AVALONMASTER_ADDRESS = address;
  assign AVM_AVALONMASTER_READ = read;
  assign AVM_AVALONMASTER_WRITE = write;
  assign AVM_AVALONMASTER_WRITEDATA = writedata;

  /****************************************************************************
  * all main function must be here or in main module. you MUST NOT use control
  * interface for the main operation and only can import and export some wires
  * from/to it
  ****************************************************************************/

  // user logic begin
  always @(posedge CSI_CLOCK_CLK)
  begin
    if(CSI_CLOCK_RESET == 0)
    begin
      done <= 0;
    end
    else
    begin
      done <= START;
    end
  end

  // user logic end

endmodule
