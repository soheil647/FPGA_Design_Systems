library verilog;
use verilog.vl_types.all;
entity ADD is
    generic(
        WIDTH           : integer := 38
    );
    port(
        INA             : in     vl_logic_vector(37 downto 0);
        INB             : in     vl_logic_vector(31 downto 0);
        \OUT\           : out    vl_logic_vector
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of WIDTH : constant is 1;
end ADD;
