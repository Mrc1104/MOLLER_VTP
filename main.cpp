#include <stdio.h>
#include <stdlib.h>
#include "moller_hls.h"

int main(int argc, char *argv[])
{
	ap_uint<3> hit_det = 2; // time tolerance for coincidence (in 4ns ticks)
	ap_uint<13> seed_threshold = 5e3;
	ap_uint<16> cluster_threshold = 10e3;
	
	// hls::stream<T> behaves like a fifo array with infinite depth
	// Once data has been read, it no longer is stored in any buffer
	hls::stream<fadc_hits_t> s_fadc_hits; // raw data stream from the 
	hls::stream<ring_data_t> s_rings; // output stream for the complete ring data

	while(!s_fadc_hits.empty()){
		moller_hls( 
			hit_det, 
			seed_threshold, 
			cluster_threshold, 
			s_fadc_hits, 
			s_rings );
	}

	while(!s_rings.empty()){
		rings_data_t tmp = s_rings.read();
		for(int i = 0; i < 8; i++){
			if()
		}
	}

	return 0;
}
