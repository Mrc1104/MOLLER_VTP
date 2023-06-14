#include "moller_hls.h"


void moller_hls
(
	ap_uint<3> hit_dt, // coincidence tolerance
	ap_uint<13> seed_threshold, // minimum energy for us to look at an individual hit
	ap_uint<16> ring_threshold, // minimum summed energy (over one ring) to count a ring as hit
	hls::stream<fadc_hits_t> &s_fadc_hits, // raw FADC data input stream
	hls::stream<trigger_t> &s_trigger, // output stream for for the trigger data
	hls::stream<ring_trigger_t> &s_ring_trigger, // output stream for for the ring trigger data
	hls::stream<ring_all_t> &s_ring_all_t // output strean for the ring data
)
{
	fadc_hits_t fadc_hits = s_fadc_hits.read();


	ap_uint<8> ac_disc[N_CHAN_SEC];
	trigger_t trigger = {0};
	ring_all_t allr;

	int segment = -1; // segments run from 0 to 4
	int sector = -1; // sectors run from 0 to 6
	for(int ch = 0; ch < N_CHAN_SEC; ch++){
		// every time ch%8 == 0, we are in a new segment
		if( (ch%8 == 0) ){
			segment++;
		}
		// every time ch%32 ==0, we are in a new sector
		if( (ch%32 == 0) ){
			sector++;
		}
		if(fadc_hits.vxs_chan[ch].e > 0 ) // else, no hit
			add_ring_data(ch, segment%4, sector, fadc_hits.vxs_chan[ch], allr);





	} // end for loop





	return;
} // void moller_hls(...)

void add_ring_data(
	int ch,
	int hit_segment, 
	int hit_sector, 
	hit_t hit_data,
	ring_all_t* ring_all
)
{
	ring_hit_t tmp; 
	tmp.e += hit_data.e;
	tmp.nhits = 1;
	tmp.sector[hit_sector] = 1;
	tmp.segment[hit_segment] = 1;


}