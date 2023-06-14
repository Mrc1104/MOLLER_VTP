#include <stdio.h>
#include <stdlib.h>

// for testing
#include <fstream>
#include <string>
#include "moller_hls.h"

int main(int argc, char *argv[])
{
	ap_uint<3> hit_dt = 2; // time tolerance for coincidence (in 4ns ticks)
	ap_uint<13> seed_threshold = 5e3;
	ap_uint<16> ring_threshold = 10e3;
	
	// hls::stream<T> behaves like a fifo array with infinite depth
	// Once data has been read, it no longer is stored in any buffer
	hls::stream<fadc_hits_t> s_fadc_hits; // raw data stream from the 
	hls::stream<trigger_t> s_trigger; // output stream for for the trigger data
	hls::stream<ring_trigger_t> s_ring_trigger; // output stream for for the ring trigger data
	hls::stream<ring_all_t> s_ring_all_t; // output stream for the ring data


	// load test data
	std::ifstream testData;
	testData.open("test_data/fake_FADC_data.txt");

	fadc_hits_t fadc_hits;
//	for(int ch = 0; ch < N_CHAN_SEC; ch++){
//		fadc_hits.vxs_chan[ch].e;
//		fadc_hits.vxs_chan[ch].t;
//	}
	do{
		int index;
		ap_uint<13> en;
		ap_uint<3> ti;

		#include <iostream>
		std::string comment;
		std::getline(testData, comment);
		testData >> index >> en >> ti;
		std::cout << index << en << ti << std::endl;
		
		fadc_hits.vxs_chan[index].e = en;
		fadc_hits.vxs_chan[index].t = ti;


	}while(!testData.eof());

	s_fadc_hits.write(fadc_hits);

	while(!s_fadc_hits.empty()){
	moller_hls
	(
		hit_dt,
		seed_threshold, 
		ring_threshold, 
		s_fadc_hits, 
		s_trigger, 
		s_ring_trigger, 
	 	s_ring_all_t 
	);
	}



	return 0;
}
