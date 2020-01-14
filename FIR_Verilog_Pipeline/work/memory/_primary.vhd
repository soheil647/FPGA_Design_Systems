library verilog;
use verilog.vl_types.all;
entity memory is
    port(
        write_data      : in     vl_logic_vector(15 downto 0);
        address         : in     vl_logic_vector(15 downto 0);
        clk             : in     vl_logic;
        mem_read        : in     vl_logic;
        mem_write       : in     vl_logic;
        read_data       : out    vl_logic_vector(15 downto 0)
    );
end memory;
