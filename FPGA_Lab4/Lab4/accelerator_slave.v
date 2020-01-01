module AVS_AVALONSLAVE_CTRL #
(
  // you can add parameters here
  // you can change these parameters
  parameter integer AVS_AVALONSLAVE_DATA_WIDTH = 32,
  parameter integer AVS_AVALONSLAVE_ADDRESS_WIDTH = 4
)
(
  // user ports begin
  output wire START,
  input wire DONE,

  // user ports end
  // dont change these ports
  input wire CSI_CLOCK_CLK,
  input wire CSI_CLOCK_RESET,
  input wire [AVS_AVALONSLAVE_ADDRESS_WIDTH - 1:0] AVS_AVALONSLAVE_ADDRESS,
  output wire AVS_AVALONSLAVE_WAITREQUEST,
  input wire AVS_AVALONSLAVE_READ,
  input wire AVS_AVALONSLAVE_WRITE,
  output wire [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] AVS_AVALONSLAVE_READDATA,
  input wire [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] AVS_AVALONSLAVE_WRITEDATA
);

  // output wires and registers
  // you can change name and type of these ports
  wire start;

  reg wait_request;
  reg [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] read_data;
  // these are slave registers. they MUST be here!
  reg [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] slv_reg0;
  reg [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] slv_reg1;
  reg [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] slv_reg2;
  reg [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] slv_reg3;

  // I/O assignment
  // never directly send values to output
  assign START = start;

  assign AVS_AVALONSLAVE_WAITREQUEST = wait_request;
  assign AVS_AVALONSLAVE_READDATA = read_data;

  // it is an example and you can change it or delete it completely
  always @(posedge CSI_CLOCK_CLK)
  begin
    // usually resets are active low but you can change its trigger type
    if(CSI_CLOCK_RESET == 0)
    begin
      slv_reg0 <= 0;
      slv_reg1 <= 0;
      slv_reg2 <= 0;
      slv_reg3 <= 0;
    end
    else if(AVS_AVALONSLAVE_WRITE)
    begin
      // address is always bytewise so must devide it by 4 for 32bit word
      case(AVS_AVALONSLAVE_ADDRESS >> 2)
      1: slv_reg0 <= AVS_AVALONSLAVE_WRITEDATA;
      2: slv_reg1 <= AVS_AVALONSLAVE_WRITEDATA;
      3: slv_reg2 <= AVS_AVALONSLAVE_WRITEDATA;
      4: slv_reg3 <= AVS_AVALONSLAVE_WRITEDATA;
      default:
      begin
        slv_reg0 <= slv_reg0;
        slv_reg1 <= slv_reg1;
        slv_reg2 <= slv_reg2;
        slv_reg3 <= slv_reg3;
      end
      endcase
    end
    // it is an example design
    else if(DONE)
    begin
      slv_reg0 <= (slv_reg0 | 32'h80000000);
    end
  end

  // do the other jobs yourself like last codes
  assign start = slv_reg0[0];

endmodule
