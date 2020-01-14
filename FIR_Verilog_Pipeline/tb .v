module tb;

	parameter INPUT_WIDTH   = 16;
	parameter INPUT_LENGTH  = 18;
	parameter INPUTS_NUMBER  = 115200;


	parameter OUTPUT_WIDTH  = 38;
	parameter OUTPUT_LENGTH = 18;

	parameter FIR_WIDTH    = 64;
	parameter COEFF_LENGTH = 7;

	reg clk, rst;

	reg  [INPUT_WIDTH - 1:0]  DATA_IN    [0: INPUTS_NUMBER - 1];
	reg  [OUTPUT_WIDTH - 1:0] CORRECT_DATA [0: INPUTS_NUMBER - 1];
	reg  [INPUT_LENGTH - 1:0] address;
	wire [INPUT_WIDTH - 1:0]  data;

	reg  [INPUT_WIDTH - 1:0]   FIR_input;
	wire [OUTPUT_WIDTH - 1:0] FIR_output;
	
  wire Done;
	reg  ready;
	integer i;
	integer invalid = 0;

	initial $readmemb("inputs.txt", DATA_IN);
	initial	$readmemb("outputs.txt", CORRECT_DATA);



  FILTER F
  (
  	.clk  (clk),
  	.FIR_input   (FIR_input),
  	.rst  (rst),
  	.ready(ready),
  	.FIR_output   (FIR_output),
  	.Done (Done)
  );

	// reset
	initial
	begin
		rst = 0;
		#50;
		rst = 1;
		#23;
		rst = 0;
	end


	// clk generation
	initial clk = 1'b0;
	always #10 clk = ~clk;


	initial
	begin
		FIR_input = 0;

		for (i = 0; i < INPUTS_NUMBER; i = i + 1)
		begin
			@(posedge clk)
			begin
				FIR_input   = DATA_IN[i];
				$display("input_data %d :    %d", i, DATA_IN[i]);
				ready = 1;


				@(posedge Done)
				begin
					if (CORRECT_DATA[i] != FIR_output)
					begin
						invalid = invalid + 1;
					end
				end
			end
		end
		$display("incorrect results: %d\n", invalid);
		#200;
		$stop;
	end
endmodule
