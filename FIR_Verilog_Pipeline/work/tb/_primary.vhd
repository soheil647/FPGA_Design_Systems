library verilog;
use verilog.vl_types.all;
entity tb is
    generic(
        INPUT_WIDTH     : integer := 16;
        INPUT_LENGTH    : integer := 18;
        INPUTS_NUMBER   : integer := 115200;
        OUTPUT_WIDTH    : integer := 38;
        OUTPUT_LENGTH   : integer := 18;
        FIR_WIDTH       : integer := 64;
        COEFF_LENGTH    : integer := 7
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of INPUT_WIDTH : constant is 1;
    attribute mti_svvh_generic_type of INPUT_LENGTH : constant is 1;
    attribute mti_svvh_generic_type of INPUTS_NUMBER : constant is 1;
    attribute mti_svvh_generic_type of OUTPUT_WIDTH : constant is 1;
    attribute mti_svvh_generic_type of OUTPUT_LENGTH : constant is 1;
    attribute mti_svvh_generic_type of FIR_WIDTH : constant is 1;
    attribute mti_svvh_generic_type of COEFF_LENGTH : constant is 1;
end tb;
