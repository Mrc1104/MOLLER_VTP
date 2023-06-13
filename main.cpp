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
	hls::stream<trigger_t> s_trigger; // stream to hold the trigger info
	hls::stream<cluster_all_t> s_cluster_all; 




	return 1;
}
