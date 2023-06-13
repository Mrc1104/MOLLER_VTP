#include "moller_hls.h"


void moller_hls
(
	ap_uint<3> hit_dt, // coincidence tolerance
	ap_uint<13> seed_threshold, // minimum energy for us to look at an individual hit
	ap_uint<16> ring_threshold, // minimum summed energy (over one ring) to count a ring as hit
	hls::stream<fadc_hits_t> &s_fadc_hits, // raw FADC data input stream
	hls::stream<ring_data_t> &rings // output stream for for the ring data
)
{
	fadc_hits_t fadc_hits = s_fadc_hits.read();
#ifndef __SYNTHESIS__
	// Initialize the simulation only (creates problems for synthesis scheduling
	static fadc_hits_t fadc_hits_pre = {{{0,0}},{{0,0}},{{0,0}}};
#else
	static fadc_hits fadc_hits_pre;
#endif
	ap_uint<8> ac_disc[N_CHAN_SEC];
	ap_uint<8> trigger = {0};
	rings_all_t allr;
	for(int ch = 0; ch < N_CHAN_SEC; ch++){

		// goal: parse which ch corresponds to which ring
		int channelMapping = ch%8;
		switch(channelMapping)
		{
		case 0:


		}





	}





	return;
} // void moller_hls(...)
