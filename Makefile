HEADER = ~/FPGA/MOLLER_VTP/include/

a.out: moller_hls.h moller_hls.cpp main.cpp
	g++ -I $(HEADER) *.cpp
