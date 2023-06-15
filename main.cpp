#include <stdio.h>
#include <stdlib.h>

// for testing
#include <fstream>
#include <string>

#include "moller_hls.h"

int main(int argc, char *argv[])
{
	ap_uint<3> hit_dt = 2; // time tolerance for coincidence (in 4ns ticks)
	// ap_uint<13> energy_threshold = 5e3;
	ap_uint<13> energy_threshold = 1;
	ap_uint<16> ring_threshold = 1;
	
	// hls::stream<T> behaves like a fifo array with infinite depth
	// Once data has been read, it no longer is stored in any buffer
	hls::stream<fadc_hits_t> s_fadc_hits; // raw data stream from the 
	hls::stream<trigger_t> s_time_trigger; // output stream for for the trigger data
	hls::stream<ring_trigger_t> s_ring_trigger; // output stream for for the ring trigger data
	hls::stream<ring_all_t> s_ring_all_t; // output stream for the ring data


	// load test data
	std::ifstream testData;
	// testData.open("test_data/fake_FADC_data.txt");
	testData.open("test_data/fake_timing_data.txt");

	fadc_hits_t fadc_hits;
	for(int ch = 0; ch < N_CHAN_SEC; ch++){
		fadc_hits.vxs_chan[ch].e = 0;
		fadc_hits.vxs_chan[ch].t = 0;
	}
	std::string comment;
	if(testData){
		std::getline(testData, comment);
		while(!testData.eof()){
			int index;
			ap_uint<13> en;
			ap_uint<3> ti;

			#include <iostream>
			testData >> index >> en >> ti;
			std::cout << index << " " << en << " "
				 	  << ti << " " << std::endl;
			fadc_hits.vxs_chan[index].e = en;
			fadc_hits.vxs_chan[index].t = ti;
		}
	}
	else{
		#include<iostream>
		std::cout << "Cannot load test data file" << std::endl;
	}
	s_fadc_hits.write(fadc_hits);

	while(!s_fadc_hits.empty()){

		moller_hls
		(
			hit_dt,
			energy_threshold, 
			ring_threshold, 
			s_fadc_hits, 
			s_time_trigger, 
			s_ring_trigger, 
	 		s_ring_all_t 
		);

	}

	while(!s_ring_all_t.empty())
	{
		ring_all_t ring_data = s_ring_all_t.read();
		for(int i = 0; i < 8; i++){
			// #include <iostream>
			// using std::cout; using std::endl;
			// cout << "ringNum: " << i << endl;
			// cout << "ring_data.r[" << i<<"].e: " << ring_data.r[i].e << endl;
			// cout << "ring_data.r[" << i<<"].nhits: " << ring_data.r[i].nhits << endl;
			// cout << "ring_data.r[" << i<<"].sector: " << ring_data.r[i].sector << endl;
			// cout << "ring_data.r[" << i<<"].segment: " << ring_data.r[i].segment << endl;
			// cout << endl;
		}
	}

	while(!s_ring_trigger.empty())
	{
		ring_trigger_t ring_trig = s_ring_trigger.read();
		for(int i = 0; i < 8; i++){
			/* IMPLEMENT*/
		}
	}

	while(!s_time_trigger.empty())
	{
		trigger_t time_trig = s_time_trigger.read();
		#include <iostream>
		using std::cout; using std::endl;
		for(int ring_index = 0; ring_index < 8; ring_index ++){
			cout << "Ring: " << ring_index << "\t";
			for(int time_tick = 7; time_tick > -1; time_tick--){
				cout << "[" << time_tick << "]";
			}
			cout << endl;
		}
	}


	return 0;
}
