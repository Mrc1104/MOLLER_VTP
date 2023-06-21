HEADER = ~/FPGA/MOLLER_VTP/include/

a.out: moller_hls.h moller_hls.cpp detector_type.h main.cpp
	g++ -I $(HEADER) moller_hls.cpp main.cpp
debug: moller_hls.h moller_hls.cpp main.cpp detector_type.h
	g++ -I $(HEADER) -g moller_hls.cpp main.cpp
