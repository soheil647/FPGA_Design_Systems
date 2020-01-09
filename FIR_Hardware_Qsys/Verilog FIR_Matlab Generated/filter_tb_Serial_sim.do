onbreak resume
onerror resume
vsim -novopt work.filter_tb_Serial
add wave sim:/filter_tb_Serial/u_filter_Serial/clk
add wave sim:/filter_tb_Serial/u_filter_Serial/clk_enable
add wave sim:/filter_tb_Serial/u_filter_Serial/reset
add wave sim:/filter_tb_Serial/u_filter_Serial/filter_in
add wave sim:/filter_tb_Serial/u_filter_Serial/filter_out
add wave sim:/filter_tb_Serial/filter_out_ref
run -all
