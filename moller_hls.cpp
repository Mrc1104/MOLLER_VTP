#include "moller_hls.h"
#include "variables.h"
#include "chan_map/array1.h"
#include "trigger_config/config_headers/all_rings_all_segs_no_scalars.h"

#include <iostream>
using std::endl; using std::cout;



void moller_hls
(
	ap_uint<13> energy_threshold, 				 // minimum energy for us to look at an individual hit
	ap_uint<16> ring_threshold, 				 // minimum summed energy (over one ring) to count a ring as hit
	hls::stream<fadc_hits_t> &s_fadc_hits, 		 // raw FADC data input stream
	hls::stream<trigger_t> &s_trigger, 			 // output stream for for the trigger data
	hls::stream<ring_trigger_t> &s_ring_trigger, // output stream for for the ring trigger data
	hls::stream<ring_all_t> &s_ring_all_t, 		 // output stream for the ring data
	hls::stream<ring_all_counter_t> &s_ring_all_counter
)
{
	fadc_hits_t fadc_hits = s_fadc_hits.read();
#ifndef __SYNTHESIS__
  // Initialize for simulation only (creates a problem for synthesis scheduling)
  static fadc_hits_t fadc_hits_pre = {0,0};
#else
  static fadc_hits_t fadc_hits_pre;
#endif
	
	ring_all_counter_t raw_counter;
	trigger_t time_bitmap;
	ring_all_t allr;
	hit_t arr_event[N_CHAN] = {0,0,0,0};
	ap_uint<8> ring_trigger_counter[8] = {0,0,0,0,0,0,0,0};
	for(int i = 0; i < 8; i++){
		allr.r[i].e = 0;
		allr.r[i].nhits = 0;
		allr.r[i].segment = 0;
		time_bitmap.trig[i] = 0;
		raw_counter.ring_counter[i].counter = 0;
		ring_trigger_counter[i] = ring_trigger_scalars[i]; 
	}
	for(int ch = 0; ch < N_CHAN; ch++){
		arr_event[ch] = make_event(fadc_hits_pre.vxs_chan[ch], fadc_hits.vxs_chan[ch]);
	}
	// set curr fadc data to previous fadc data
	fadc_hits_pre = fadc_hits;

	for(int ch = 0; ch < N_CHAN; ch++){
		if(arr_event[ch].e >= energy_threshold ){ // else, no hit

			/* Need to determine which channel corresponds to which slot / fadc channel */
			int ich = ch%16; // channel # inside the fadc (starts at 0)
			int slot = (ch-ich)/16; // slot # (starts at 0)

			/* Get Channel to Detector Mappig Information */
			int ring_num = chmap[slot][ich].DET_ID; // Ring_number is labeled starting at 1 but indexed starting at 0
	      	int segment_num = chmap[slot][ich].SEG_NUM;
			int sub_element = chmap[slot][ich].SUB_ELEMENT;
			if(ring_num == NONE) { continue; } // ring_num == 0 => DET_ID == NONE
			if(ring_num == RING_FIVE){ // careful! ring 5 is actually 3 rings (5a->index 5, 5b->index 6, 5c->7) thus ring 6->index8
				if(sub_element == 'A') { ring_num = 5; }
				else if(sub_element == 'B') { ring_num = 6; }
				else if(sub_element == 'C') { ring_num = 7; }
			}
			else if(ring_num == RING_SIX) { ring_num = 8; }

			// TODO: computers start counting at 0 so ring_num - 1 is the appropriate index
			// make this a bit manip check with a bit map (see stack exhange)
			if( (ring_trigger_config_bitmap[ring_num-1] == 0) || (segment_trigger_config_bitmap[ring_num-1][segment_num] == 0) ){
				continue;
			}
			raw_counter.ring_counter[ring_num-1].counter++;
			if(ring_trigger_counter[ring_num-1] == 0){
				add_ring_data(ring_num-1, segment_num, arr_event[ch], allr.r);
				make_timing_bitmap(ring_num-1, arr_event[ch], &time_bitmap);
				ring_trigger_counter[ring_num-1] = ring_trigger_scalars[ring_num-1];
			}
			else{
				ring_trigger_counter[ring_num-1]--;
			}
		}
	} // end for loop

	ring_trigger_t ring_bitmap = make_ring_bitmap(allr.r,ring_threshold);
	s_ring_all_counter.write(raw_counter);
	s_ring_all_t.write(allr);
	s_ring_trigger.write(ring_bitmap);
	s_trigger.write(time_bitmap);


	return;
} // void moller_hls(...)

hit_t make_event(
	hit_t pre_hit, 
	hit_t cur_hit
)
{
	hit_t tmp = {0,0};
	if(pre_hit.t >=4)
		tmp = pre_hit;
	else if(cur_hit.t < 4)
		tmp = cur_hit;
	return tmp;
}

void add_ring_data(
	int ringNum,
	int hit_segment,
	hit_t hit_data,
	ring_hit_t* rings
)
{
	rings[ringNum].e += hit_data.e;
	rings[ringNum].nhits += 1;
	rings[ringNum].segment = (1<<hit_segment) | rings[ringNum].segment;
}

ring_trigger_t make_ring_bitmap(ring_hit_t* rings, ap_uint<16> ring_threshold)
{
	ring_trigger_t tmp;
	for(int ringNum = 0; ringNum < 8; ringNum++){
		if(rings[ringNum].e >= ring_threshold){
			tmp.ring = (1<<ringNum) | tmp.ring;
		}
		else{
			tmp.ring[ringNum] = 0; // just being explicit about it
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

	ptrigger->trig[ring_num] = (1<<t_actual) | ptrigger->trig[ring_num];
	
}
