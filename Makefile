comp:
	iverilog-vpi vsystem.c
	iverilog -o tb_vsystem  tb_vsystem.v

run:
	vvp -M. -mvsystem tb_vsystem

all: comp run
