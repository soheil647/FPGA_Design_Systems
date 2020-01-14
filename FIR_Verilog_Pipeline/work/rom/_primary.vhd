library verilog;
use verilog.vl_types.all;
entity rom is
    generic(
        WIDTH           : integer := 16;
        LENGHT          : integer := 64
    );
    port(
        clk             : in     vl_logic;
        rst             : in     vl_logic;
        ADR             : in     vl_logic_vector(15 downto 0);
        DATA            : out    vl_logic_vector
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of WIDTH : constant is 1;
    attribute mti_svvh_generic_type of LENGHT : constant is 1;
end rom;
