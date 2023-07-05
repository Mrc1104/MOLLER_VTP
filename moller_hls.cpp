#include "moller_hls.h"
#include <hls_vector.h>

#include <iostream>
using std::endl; using std::cout;

void moller_hls
(
	ap_uint<3> hit_dt, 							 // coincidence tolerance
	ap_uint<13> energy_threshold, 				 // minimum energy for us to look at an individual hit
	ap_uint<16> ring_threshold, 				 // minimum summed energy (over one ring) to count a ring as hit
	chan_map_integer arr_chan_map_One[][16], 	 // array that maps the channel to detector
	chan_map_integer arr_chan_map_Two[][16],
	hls::stream<fadc_hits_t> &s_fadc_hits, 		 // raw FADC data input stream
	hls::stream<trigger_t> &s_trigger, 			 // output stream for for the trigger data
	hls::stream<ring_trigger_t> &s_ring_trigger, // output stream for for the ring trigger data
	hls::stream<ring_all_t> &s_ring_all_t 		 // output stream for the ring data
)
{
	fadc_hits_t fadc_hits = s_fadc_hits.read();


	trigger_t time_bitmap;
	ring_all_t allr;
	for(int i = 0; i < 8; i++){
		allr.r[i].e = 0;
		allr.r[i].nhits = 0;
		allr.r[i].segment = 0;
		for(int j = 0; j < 8; j++){
			// trigger_t time_bitmap = {0}; 
			/* The XILINX documentation says not to initialize ap_uint<N> like above */
			/* and that if used in an array, they might not initialize to zero       */
			/* So I am initializing the array if ap_uint<N> explicitly               */
			time_bitmap.trig[i] = 0;
		}
	}
	


	for(int ch = 0; ch < N_CHAN_SEC; ch++){
		if(fadc_hits.vxs_chan[ch].e >= energy_threshold ){ // else, no hit
			/* Need to determine which channel corresponds to which slot / fadc channel */
			int ich = ch%16; // channel # inside the fadc (starts at 0)
			int slot = (ch-ich)/16; // slot # (starts at 0)

			/* Get Channel to Detector Mappig Information */
			int ring_num;
      		int segment_num;
			int sub_element;
			if( (slot % 2) == 0){
				// we start counting ring_num at 0
				ring_num = arr_chan_map_One[slot][ich].DET_ID - 1;
	      		segment_num = arr_chan_map_One[slot][ich].SEG_NUM - 1;
				sub_element = arr_chan_map_One[slot][ich].SUB_ELEMENT -1 ;
			}
			else{
				// we start counting ring_num at 0
				ring_num = arr_chan_map_Two[slot][ich].DET_ID - 1;
	      		segment_num = arr_chan_map_Two[slot][ich].SEG_NUM - 1;
				sub_element = arr_chan_map_Two[slot][ich].SUB_ELEMENT -1;
			}
			if(ring_num == -1) { continue; } // ring_num == -1 => DET_ID == NONE
			if(ring_num == 4){
				if(sub_element == 'A') { ring_num = 4; }
				else if(sub_element == 'B') { ring_num = 5; }
				else if(sub_element == 'C') { ring_num = 6; }
			}
//				add_ring_data(segment_num, fadc_hits.vxs_chan[ch], &allr.r[ringNum]);
//				add_ring_data(fadc_hits.vxs_chan[ch], &allr.r[ringNum]);
				add_ring_data(ring_num, segment_num, fadc_hits.vxs_chan[ch], allr.r);
				make_timing_bitmap(ring_num, fadc_hits.vxs_chan[ch], &time_bitmap);
		}
	} // end for loop

	ring_trigger_t ring_bitmap = make_ring_bitmap(allr.r,ring_threshold);



	s_ring_all_t.write(allr);
	s_ring_trigger.write(ring_bitmap);
	s_trigger.write(time_bitmap);


	return;
} // void moller_hls(...)

//void add_ring_data(
//	int hit_segment,
//	hit_t hit_data,
//	ring_hit_t *rings
//)
//{
//	rings->e += hit_data.e;
//	rings->nhits++;
//	rings->segment[hit_segment] = 1;
//}
void add_ring_data(
	int ringNum,
	int hit_segment,
	hit_t hit_data,
	ring_hit_t* rings
)
{


	rings[ringNum].e += hit_data.e;
	rings[ringNum].nhits += 1;
	rings[ringNum].segment[hit_segment] = 1;
}

ring_trigger_t make_ring_bitmap(ring_hit_t* rings, ap_uint<16> ring_threshold)
{
	ring_trigger_t tmp;
	for(int ringNum = 0; ringNum < 8; ringNum++){
		if(rings[ringNum].e >= ring_threshold){
			tmp.ring[ringNum] = 1;
		}
		else{
			tmp.ring[ringNum] = 0; // just being explicit about it
			// Interesting... if our ring does not meet the threshold to have a "hit", should we set all the other bit maps to 0?
		}
	}
	// TODO: WE COULD ALSO ADD NHIT FILTERING HERE TOO

	return tmp;
}

void make_timing_bitmap(int ring_num, hit_t hit_data, trigger_t *ptrigger)
{

	ap_uint<4> t_buff=0;
	if(hit_data.t >=4)
		t_buff = hit_data.t; // map pre time 4 to 7 -> 4 to 7 (unchanged)
	else if(hit_data.t < 4)
		t_buff = hit_data.t + 8; // map cur time 0 to 3 -> 8 to 11 (move to time after pre hit window)
	ap_uint<3> t_actual = t_buff - 4;

	ptrigger->trig[ring_num][t_actual] = 1;
}
