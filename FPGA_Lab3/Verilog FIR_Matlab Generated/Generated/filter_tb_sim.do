onbreak resume
onerror resume
vsim -novopt work.filter_tb
add wave sim:/filter_tb/u_filter_Serial/clk
add wave sim:/filter_tb/u_filter_Serial/clk_enable
add wave sim:/filter_tb/u_filter_Serial/reset
add wave sim:/filter_tb/u_filter_Serial/filter_in
add wave sim:/filter_tb/u_filter_Serial/filter_out
add wave sim:/filter_tb/filter_out_ref
run -all
