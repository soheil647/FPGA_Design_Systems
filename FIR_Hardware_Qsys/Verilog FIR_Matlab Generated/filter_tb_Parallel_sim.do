onbreak resume
onerror resume
vsim -novopt work.filter_tb_Parallel
add wave sim:/filter_tb_Parallel/u_filter_Parallel/clk
add wave sim:/filter_tb_Parallel/u_filter_Parallel/clk_enable
add wave sim:/filter_tb_Parallel/u_filter_Parallel/reset
add wave sim:/filter_tb_Parallel/u_filter_Parallel/filter_in
add wave sim:/filter_tb_Parallel/u_filter_Parallel/filter_out
add wave sim:/filter_tb_Parallel/filter_out_ref
run -all
