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
  input wire Go,
  output wire DONE,
  input wire[18:0] Size,
  input wire[10:0] Number,
  input wire [AVM_AVALONMASTER_DATA_WIDTH - 1:0] slv_reg1,
  input wire [AVM_AVALONMASTER_DATA_WIDTH - 1:0] slv_reg2,
  input wire [AVM_AVALONMASTER_DATA_WIDTH - 1:0] slv_reg3,
  input Master_Done,

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

  //parameters
  localparam  Wait_For_Go = 1;
  localparam  Read_Left_Register = 2;
  localparam  Read_Right_Register = 3;
  localparam  Do_Sum = 4;
  localparam  Send_32MSB = 5;
  localparam  Send_32LSB = 6;
  localparam  Done_State = 7;

  // output wires and registers
  // you can change name and type of these ports
  reg done;

  reg [AVM_AVALONMASTER_ADDRESS_WIDTH - 1:0] address;
  reg read;
  reg write;
  reg [AVM_AVALONMASTER_DATA_WIDTH - 1:0] writedata;

  //My registers
  reg[2:0] Now_State;
  wire Wait_Request;
  reg[AVM_AVALONMASTER_DATA_WIDTH - 1:0] Left_Reg_Data;
  reg[AVM_AVALONMASTER_DATA_WIDTH - 1:0] Right_Reg_Data;
  wire[AVM_AVALONMASTER_DATA_WIDTH - 1:0] Read_Data;
  reg[2 * AVM_AVALONMASTER_DATA_WIDTH - 1:0] Sum_Reg;
  reg[10:0] Number_Count;
  reg[18:0] Size_Count;
  // I/O assignment
  // never directly send values to output
  assign DONE = done;
  assign AVM_AVALONMASTER_ADDRESS = address;
  assign AVM_AVALONMASTER_READ = read;
  assign AVM_AVALONMASTER_WRITE = write;
  assign AVM_AVALONMASTER_WRITEDATA = writedata;

  //My Assigns
  assign Wait_Request = AVM_AVALONMASTER_WAITREQUEST;
  assign Read_Data = AVM_AVALONMASTER_READDATA;

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
      done <= 1'b0;
      Sum_Reg <= 0;
      read <= 1'b0;
      write <= 1'b0;
      address <= 0;
      Left_Reg_Data <= 0;
      Right_Reg_Data <= 0;
      Number_Count <= 11'b0;
      Size_Count <= 19'b0;
      writedata <= 0;
      Now_State <= Wait_For_Go;
    end
    else
    begin
      case(Now_State)
        Wait_For_Go:begin
            if(Wait_Request) begin
                Now_State <= Wait_For_Go;
            end
            else begin
                if(Go) begin
                    Now_State <= Read_Left_Register;
                    done <= 1'b0;
                    read <= 1'b1;
                    write <= 1'b0;
                    address <= slv_reg2 + Size_Count << 2;
                end
            end
        end

        Read_Left_Register:begin
            if(Wait_Request) begin
                Now_State <= Read_Left_Register;
            end
            else begin
                Now_State <= Read_Right_Register;
                Left_Reg_Data <= Read_Data;
                read <= 1'b1;
                write <= 1'b0;
                done <= 1'b0;
                address <= slv_reg1 + Size_Count << 2;
            end
        end

        Read_Right_Register:begin
            if(Wait_Request) begin
                Now_State <= Read_Right_Register;
            end
            else begin
                Now_State <= Do_Sum;
                read <= 1'b0;
                write <= 1'b0;
                done <= 1'b0;
                address <= 1'b0;
                Right_Reg_Data <= Read_Data;
            end
        end

        Do_Sum:begin
            if(Wait_Request) begin
                Now_State <= Do_Sum;
            end
            else if(Size_Count <= Size) begin
                Size_Count <= Size_Count + 1;
                Sum_Reg <= Left_Reg_Data + Right_Reg_Data + Sum_Reg;
                read <= 1'b1;
                write <= 1'b0;
                done <= 1'b0;
                address <= slv_reg2 + Size_Count << 2;
                Now_State <= Read_Left_Register;
            end
            else begin
                Now_State <= Send_32MSB;
                Size_Count <= 0;
                read <= 1'b0;
                write <= 1'b1;
                done <= 1'b0;
                address <= slv_reg3 + Number_Count << 2;
                writedata <= Sum_Reg[2 * AVM_AVALONMASTER_DATA_WIDTH - 1:AVM_AVALONMASTER_DATA_WIDTH];
            end
        end

        Send_32MSB:begin
            if(Wait_Request) begin
                Now_State <= Send_32MSB;
            end
            else begin
                Now_State <= Send_32LSB;
                read <= 1'b0;
                write <= 1'b1;
                done <= 1'b0;
                address <= slv_reg3 + 4 + Number_Count << 2;
                writedata <= Sum_Reg[AVM_AVALONMASTER_DATA_WIDTH - 1:0];
            end
        end

        Send_32LSB:begin
            if(Wait_Request) begin
                Now_State <= Send_32LSB;
            end
            else if(Number_Count <= Number) begin
                Now_State <= Read_Left_Register;
                Number_Count <= Number_Count + 1;
                write <= 1'b0;
                read <= 1'b1;
                done <= 1'b0;
                address <= slv_reg2 + Size_Count << 2;
            end
            else begin
                Now_State <= Done_State;
            end
        end

        Done_State:begin
            done <= 1'b1;
            write <= 1'b0;
            read <= 1'b0;
            address <= 1'b0;
            Number_Count <= 0;
            if(Master_Done)begin
                Now_State <= Wait_For_Go;
            end
            else begin
                Now_State <= Done_State;
            end
        end
      endcase
    end
  end
  // user logic end

endmodule
