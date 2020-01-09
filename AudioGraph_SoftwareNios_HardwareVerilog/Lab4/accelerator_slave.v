module AVS_AVALONSLAVE_CTRL #
(
  // you can add parameters here
  // you can change these parameters
  parameter integer AVS_AVALONSLAVE_DATA_WIDTH = 32,
  parameter integer AVS_AVALONSLAVE_ADDRESS_WIDTH = 4
)
(
  // user ports begin
  output wire Go,
  input wire DONE,
  output wire [10:0] Number_Out,
  output wire [18:0] Size_Out,
  output wire [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] slv_reg1_out,
  output wire [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] slv_reg2_out,
  output wire [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] slv_reg3_out,
  output wire Slave_Done,
  input wire[2:0] Temp1_State,

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

  wire wait_request;
  reg [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] read_data;
  // these are slave registers. they MUST be here!
  reg [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] slv_reg0;
  reg [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] slv_reg1;
  reg [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] slv_reg2;
  reg [AVS_AVALONSLAVE_DATA_WIDTH - 1:0] slv_reg3;

  reg [2:0] Temp_Slave_State;
  //My registers
  wire[10:0] Number;
  wire[18:0] Size;
  // I/O assignment
  // never directly send values to output

  assign AVS_AVALONSLAVE_WAITREQUEST = wait_request;
  assign AVS_AVALONSLAVE_READDATA = read_data;
  //My Assigns
  assign Number_Out = Number;
  assign Size_Out = Size;
  assign slv_reg1_out = slv_reg1;
  assign slv_reg2_out = slv_reg2;
  assign slv_reg3_out = slv_reg3;
  assign wait_request = 0;

  // it is an example and you can change it or delete it completely
  always @(posedge CSI_CLOCK_CLK)
  begin
    Temp_Slave_State <= Temp1_State;
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
      0: slv_reg0 <= AVS_AVALONSLAVE_WRITEDATA;
      1: slv_reg1 <= AVS_AVALONSLAVE_WRITEDATA;
      2: slv_reg2 <= AVS_AVALONSLAVE_WRITEDATA;
      3: slv_reg3 <= AVS_AVALONSLAVE_WRITEDATA;
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
      slv_reg0[31] <= 1'b1;
      slv_reg0[0] <= 1'b0;
    end
  end

  always@(*)begin
    if(AVS_AVALONSLAVE_READ)
    begin
      // address is always bytewise so must devide it by 4 for 32bit word
      case(AVS_AVALONSLAVE_ADDRESS >> 2)
      0: read_data = slv_reg0;
      1: read_data = slv_reg1;
      2: read_data = slv_reg2;
      3: read_data = slv_reg3;
      default:
      begin
        read_data = 0;
      end
      endcase
    end
    else begin
        read_data = 0;
    end
  end

  // do the other jobs yourself like last codes
  assign Go = slv_reg0[0];
  assign Number = slv_reg0[11:1];
  assign Size = slv_reg0[30:12];
  assign Slave_Done = slv_reg0[31];

endmodule
