library verilog;
use verilog.vl_types.all;
entity CU is
    generic(
        one             : vl_logic_vector(2 downto 0) := (Hi0, Hi0, Hi0);
        two             : vl_logic_vector(2 downto 0) := (Hi0, Hi0, Hi1);
        three           : vl_logic_vector(2 downto 0) := (Hi0, Hi1, Hi0);
        four            : vl_logic_vector(2 downto 0) := (Hi0, Hi1, Hi1);
        five            : vl_logic_vector(2 downto 0) := (Hi1, Hi0, Hi0)
    );
    port(
        clk             : in     vl_logic;
        rst             : in     vl_logic;
        ready           : in     vl_logic;
        N               : in     vl_logic_vector(15 downto 0);
        done            : out    vl_logic;
        write           : out    vl_logic;
        read            : out    vl_logic;
        init_reg        : out    vl_logic;
        ld              : out    vl_logic;
        clear_K         : out    vl_logic;
        cn_N            : out    vl_logic;
        cn_K            : out    vl_logic;
        clear_N         : out    vl_logic
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of one : constant is 2;
    attribute mti_svvh_generic_type of two : constant is 2;
    attribute mti_svvh_generic_type of three : constant is 2;
    attribute mti_svvh_generic_type of four : constant is 2;
    attribute mti_svvh_generic_type of five : constant is 2;
end CU;
