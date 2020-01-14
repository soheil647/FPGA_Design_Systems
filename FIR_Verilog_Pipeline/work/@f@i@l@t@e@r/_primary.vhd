library verilog;
use verilog.vl_types.all;
entity FILTER is
    generic(
        LENGTH          : integer := 38;
        WIDTH           : integer := 16
    );
    port(
        FIR_input       : in     vl_logic_vector;
        clk             : in     vl_logic;
        rst             : in     vl_logic;
        ready           : in     vl_logic;
        FIR_output      : out    vl_logic_vector;
        Done            : out    vl_logic
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of LENGTH : constant is 1;
    attribute mti_svvh_generic_type of WIDTH : constant is 1;
end FILTER;
