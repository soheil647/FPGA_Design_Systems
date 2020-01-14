library verilog;
use verilog.vl_types.all;
entity dp is
    port(
        X_in            : in     vl_logic_vector(15 downto 0);
        clk             : in     vl_logic;
        rst             : in     vl_logic;
        clear_k         : in     vl_logic;
        ce_k            : in     vl_logic;
        Write           : in     vl_logic;
        Read            : in     vl_logic;
        Load            : in     vl_logic;
        init_reg        : in     vl_logic;
        ce_n            : in     vl_logic;
        clear_n         : in     vl_logic;
        \out\           : out    vl_logic_vector(37 downto 0);
        out_N           : out    vl_logic_vector(15 downto 0)
    );
end dp;
