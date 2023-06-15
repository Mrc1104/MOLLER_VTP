HEADER = ~/FPGA/MOLLER_VTP/include/

a.out: moller_hls.h moller_hls.cpp main.cpp
	g++ -I $(HEADER) moller_hls.cpp main.cpp
genData: moller_hls.h moller_hls.cpp generateFakeData.cpp
	g++ -o fakedata.out -I $(HEADER) moller_hls.cpp generateFakeData.cpp
	./fakedata.out
debug: moller_hls.h moller_hls.cpp main.cpp
	g++ -I $(HEADER) -g moller_hls.cpp main.cpp
