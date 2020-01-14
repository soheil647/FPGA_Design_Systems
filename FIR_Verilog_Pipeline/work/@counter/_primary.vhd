library verilog;
use verilog.vl_types.all;
entity Counter is
    port(
        clk             : in     vl_logic;
        rst             : in     vl_logic;
        count_up        : in     vl_logic;
        clear           : in     vl_logic;
        w               : out    vl_logic_vector(15 downto 0)
    );
end Counter;
