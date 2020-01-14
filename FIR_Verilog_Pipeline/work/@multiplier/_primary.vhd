library verilog;
use verilog.vl_types.all;
entity Multiplier is
    generic(
        WIDTH_IN        : integer := 16;
        WIDTH_OUT       : integer := 32
    );
    port(
        INA             : in     vl_logic_vector;
        INB             : in     vl_logic_vector;
        \OUT\           : out    vl_logic_vector
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of WIDTH_IN : constant is 1;
    attribute mti_svvh_generic_type of WIDTH_OUT : constant is 1;
end Multiplier;
