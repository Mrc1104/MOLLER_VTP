#include <stdio.h>
#include <stdlib.h>

// for testing
#include <fstream>
#include <string>
#include "moller_hls.h"

int main(int argc, char *argv[])
{
	ap_uint<3> hit_det = 2; // time tolerance for coincidence (in 4ns ticks)
	ap_uint<13> seed_threshold = 5e3;
	ap_uint<16> cluster_threshold = 10e3;
	
	// hls::stream<T> behaves like a fifo array with infinite depth
	// Once data has been read, it no longer is stored in any buffer
	hls::stream<fadc_hits_t> s_fadc_hits; // raw data stream from the 
	hls::stream<trigger_t> s_trigger; // output stream for for the trigger data
	hls::stream<ring_trigger_t> s_ring_trigger; // output stream for for the ring trigger data
	hls::stream<ring_all_t> s_ring_all_t; // output strean for the ring data


	// load test data
	std::ifstream testData;
	testData.open("test_data/fake_FADC_data.txt");
	do{
		#include <iostream>
		std::string str;
		std::getline(testData, str);
		std::cout << str << std::endl;

	}while(!testData.eof());



	while(!s_fadc_hits.empty()){
		/* moller_hls( 
			hit_det, 
			seed_threshold, 
			cluster_threshold, 
			s_fadc_hits, 
			s_trigger,
			s_ring_trigger,
			s_ring_all_t ); */
	}



	return 0;
}
