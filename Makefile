HEADER = ~/FPGA/MOLLER_VTP/include/

a.out: moller_hls.h moller_hls.cpp detector_type.h chan_map.h main.cpp
	g++ -I $(HEADER) -Wfatal-errors moller_hls.cpp main.cpp
debug: moller_hls.h moller_hls.cpp main.cpp detector_type.h chan_map.h
	g++ -I $(HEADER) -g moller_hls.cpp main.cpp
