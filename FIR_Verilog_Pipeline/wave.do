onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tb/clk
add wave -noupdate -radix unsigned /tb/F/CONTROLLER/ps
add wave -noupdate /tb/rst
add wave -noupdate -radix unsigned -childformat {{{/tb/DATA_IN[0]} -radix unsigned} {{/tb/DATA_IN[1]} -radix unsigned} {{/tb/DATA_IN[2]} -radix unsigned} {{/tb/DATA_IN[3]} -radix unsigned} {{/tb/DATA_IN[4]} -radix unsigned} {{/tb/DATA_IN[5]} -radix unsigned} {{/tb/DATA_IN[6]} -radix unsigned} {{/tb/DATA_IN[7]} -radix unsigned} {{/tb/DATA_IN[8]} -radix unsigned} {{/tb/DATA_IN[9]} -radix unsigned} {{/tb/DATA_IN[10]} -radix unsigned} {{/tb/DATA_IN[11]} -radix unsigned} {{/tb/DATA_IN[12]} -radix unsigned} {{/tb/DATA_IN[13]} -radix unsigned} {{/tb/DATA_IN[14]} -radix unsigned} {{/tb/DATA_IN[15]} -radix unsigned} {{/tb/DATA_IN[16]} -radix unsigned} {{/tb/DATA_IN[17]} -radix unsigned}} -subitemconfig {{/tb/DATA_IN[0]} {-height 15 -radix unsigned} {/tb/DATA_IN[1]} {-height 15 -radix unsigned} {/tb/DATA_IN[2]} {-height 15 -radix unsigned} {/tb/DATA_IN[3]} {-height 15 -radix unsigned} {/tb/DATA_IN[4]} {-height 15 -radix unsigned} {/tb/DATA_IN[5]} {-height 15 -radix unsigned} {/tb/DATA_IN[6]} {-height 15 -radix unsigned} {/tb/DATA_IN[7]} {-height 15 -radix unsigned} {/tb/DATA_IN[8]} {-height 15 -radix unsigned} {/tb/DATA_IN[9]} {-height 15 -radix unsigned} {/tb/DATA_IN[10]} {-height 15 -radix unsigned} {/tb/DATA_IN[11]} {-height 15 -radix unsigned} {/tb/DATA_IN[12]} {-height 15 -radix unsigned} {/tb/DATA_IN[13]} {-height 15 -radix unsigned} {/tb/DATA_IN[14]} {-height 15 -radix unsigned} {/tb/DATA_IN[15]} {-height 15 -radix unsigned} {/tb/DATA_IN[16]} {-height 15 -radix unsigned} {/tb/DATA_IN[17]} {-height 15 -radix unsigned}} /tb/DATA_IN
add wave -noupdate -radix unsigned -childformat {{{/tb/CORRECT_DATA[0]} -radix unsigned} {{/tb/CORRECT_DATA[1]} -radix unsigned} {{/tb/CORRECT_DATA[2]} -radix unsigned} {{/tb/CORRECT_DATA[3]} -radix unsigned} {{/tb/CORRECT_DATA[4]} -radix unsigned} {{/tb/CORRECT_DATA[5]} -radix unsigned} {{/tb/CORRECT_DATA[6]} -radix unsigned} {{/tb/CORRECT_DATA[7]} -radix unsigned} {{/tb/CORRECT_DATA[8]} -radix unsigned} {{/tb/CORRECT_DATA[9]} -radix unsigned} {{/tb/CORRECT_DATA[10]} -radix unsigned} {{/tb/CORRECT_DATA[11]} -radix unsigned} {{/tb/CORRECT_DATA[12]} -radix unsigned} {{/tb/CORRECT_DATA[13]} -radix unsigned} {{/tb/CORRECT_DATA[14]} -radix unsigned} {{/tb/CORRECT_DATA[15]} -radix unsigned} {{/tb/CORRECT_DATA[16]} -radix unsigned} {{/tb/CORRECT_DATA[17]} -radix unsigned}} -subitemconfig {{/tb/CORRECT_DATA[0]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[1]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[2]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[3]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[4]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[5]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[6]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[7]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[8]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[9]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[10]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[11]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[12]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[13]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[14]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[15]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[16]} {-height 15 -radix unsigned} {/tb/CORRECT_DATA[17]} {-height 15 -radix unsigned}} /tb/CORRECT_DATA
add wave -noupdate -radix unsigned /tb/FIR_input
add wave -noupdate -radix binary /tb/FIR_output
add wave -noupdate /tb/Done
add wave -noupdate /tb/ready
add wave -noupdate -expand -group multiplier -radix unsigned /tb/F/DATAPATH/M/INA
add wave -noupdate -expand -group multiplier -radix unsigned /tb/F/DATAPATH/M/INB
add wave -noupdate -expand -group multiplier -radix unsigned /tb/F/DATAPATH/M/OUT
add wave -noupdate -expand -group adder -radix unsigned /tb/F/DATAPATH/Adder/INA
add wave -noupdate -expand -group adder -radix unsigned /tb/F/DATAPATH/Adder/INB
add wave -noupdate -expand -group adder -radix unsigned /tb/F/DATAPATH/Adder/OUT
add wave -noupdate -expand -group reg /tb/F/DATAPATH/Reg/ld
add wave -noupdate -expand -group reg /tb/F/DATAPATH/Reg/rst
add wave -noupdate -expand -group reg -radix unsigned /tb/F/DATAPATH/Reg/d
add wave -noupdate -expand -group reg -radix unsigned /tb/F/DATAPATH/Reg/q
add wave -noupdate -group rom -radix unsigned /tb/F/DATAPATH/ROM/coeffs
add wave -noupdate -group rom -radix unsigned /tb/F/DATAPATH/ROM/ADR
add wave -noupdate -group ram -radix unsigned -childformat {{{/tb/F/DATAPATH/Memory/reg_mem[0][15]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][14]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][13]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][12]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][11]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][10]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][9]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][8]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][7]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][6]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][5]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][4]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][3]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][2]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][1]} -radix unsigned} {{/tb/F/DATAPATH/Memory/reg_mem[0][0]} -radix unsigned}} -subitemconfig {{/tb/F/DATAPATH/Memory/reg_mem[0][15]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][14]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][13]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][12]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][11]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][10]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][9]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][8]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][7]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][6]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][5]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][4]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][3]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][2]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][1]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/reg_mem[0][0]} {-height 15 -radix unsigned}} {/tb/F/DATAPATH/Memory/reg_mem[0]}
add wave -noupdate -group ram -radix unsigned {/tb/F/DATAPATH/Memory/reg_mem[1]}
add wave -noupdate -group ram -radix unsigned {/tb/F/DATAPATH/Memory/reg_mem[2]}
add wave -noupdate -group ram -radix unsigned {/tb/F/DATAPATH/Memory/reg_mem[3]}
add wave -noupdate -group ram -radix unsigned {/tb/F/DATAPATH/Memory/reg_mem[4]}
add wave -noupdate -group ram /tb/F/DATAPATH/Memory/mem_write
add wave -noupdate -group ram /tb/F/DATAPATH/Memory/mem_read
add wave -noupdate -group ram -radix unsigned /tb/F/DATAPATH/Memory/read_data
add wave -noupdate -group ram -radix decimal -childformat {{{/tb/F/DATAPATH/Memory/address[15]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[14]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[13]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[12]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[11]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[10]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[9]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[8]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[7]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[6]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[5]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[4]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[3]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[2]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[1]} -radix unsigned} {{/tb/F/DATAPATH/Memory/address[0]} -radix unsigned}} -subitemconfig {{/tb/F/DATAPATH/Memory/address[15]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[14]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[13]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[12]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[11]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[10]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[9]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[8]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[7]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[6]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[5]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[4]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[3]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[2]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[1]} {-height 15 -radix unsigned} {/tb/F/DATAPATH/Memory/address[0]} {-height 15 -radix unsigned}} /tb/F/DATAPATH/Memory/address
add wave -noupdate -group ram -radix unsigned /tb/F/DATAPATH/Memory/write_data
add wave -noupdate -group {cnt K} /tb/F/DATAPATH/K/count_up
add wave -noupdate -group {cnt K} /tb/F/DATAPATH/K/clear
add wave -noupdate -group {cnt K} -radix unsigned /tb/F/DATAPATH/K/w
add wave -noupdate -group {cnt N} /tb/F/DATAPATH/N/count_up
add wave -noupdate -group {cnt N} /tb/F/DATAPATH/N/clear
add wave -noupdate -group {cnt N} -radix unsigned /tb/F/DATAPATH/N/w
add wave -noupdate -radix decimal /tb/F/DATAPATH/out_N
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {580 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 264
configure wave -valuecolwidth 257
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {344 ps} {626 ps}
