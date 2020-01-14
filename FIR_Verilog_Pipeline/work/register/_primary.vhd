library verilog;
use verilog.vl_types.all;
entity \register\ is
    port(
        d               : in     vl_logic_vector(37 downto 0);
        clk             : in     vl_logic;
        rst             : in     vl_logic;
        ld              : in     vl_logic;
        q               : out    vl_logic_vector(37 downto 0)
    );
end \register\;
