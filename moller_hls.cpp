#include "moller_hls.h"


void moller_hls
(
	ap_uint<3> hit_dt, // coincidence tolerance
	ap_uint<13> energy_threshold, // minimum energy for us to look at an individual hit
	ap_uint<16> ring_threshold, // minimum summed energy (over one ring) to count a ring as hit
	hls::stream<fadc_hits_t> &s_fadc_hits, // raw FADC data input stream
	hls::stream<trigger_t> &s_trigger, // output stream for for the trigger data
	hls::stream<ring_trigger_t> &s_ring_trigger, // output stream for for the ring trigger data
	hls::stream<ring_all_t> &s_ring_all_t // output stream for the ring data
)
{
	fadc_hits_t fadc_hits = s_fadc_hits.read();


	ap_uint<8> ac_disc[N_CHAN_SEC];
	trigger_t time_bitmap = {0};
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			time_bitmap.trig[i] = 0;
		}
	}
	ring_all_t allr;

	for(int i = 0; i < 8; i++){
		allr.r[i].e = 0;
		allr.r[i].nhits = 0;
		allr.r[i].sector = {0};
		allr.r[i].segment = {0};
	}

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
		if(fadc_hits.vxs_chan[ch].e >= energy_threshold ){ // else, no hit
      int ring_num = ch%8;
      int segment_num = segment%4; 
			add_ring_data(ring_num, segment_num, sector, fadc_hits.vxs_chan[ch], allr.r);
			make_timing_bitmap(ring_num, fadc_hits.vxs_chan[ch], &time_bitmap);
		}
	} // end for loop

	ring_trigger_t ring_bitmap = make_ring_bitmap(allr.r, ring_threshold);



	s_ring_all_t.write(allr);
	s_ring_trigger.write(ring_bitmap);

	return;
} // void moller_hls(...)

void add_ring_data(
	int ringNum,
	int hit_segment, 
	int hit_sector, 
	hit_t hit_data,
	ring_hit_t* rings
)
{
	#include <iostream>
	using std::cout; using std::endl;
	
	rings[ringNum].e += hit_data.e;
	rings[ringNum].nhits += 1;
	rings[ringNum].sector[hit_sector] = 1;
	rings[ringNum].segment[hit_segment] = 1;

	// cout << "ringNum: " << ringNum << endl;
	// cout << "rings[" << ringNum<<"].e: " << rings[ringNum].e << endl;
	// cout << "rings[" << ringNum<<"].nhits: " << rings[ringNum].nhits << endl;
	// cout << "rings[" << ringNum<<"].sector: " << rings[ringNum].sector << endl;
	// cout << "rings[" << ringNum<<"].segment: " << rings[ringNum].segment << endl;
	// cout << endl;

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
		}
	}
	// TODO: WE COULD ALSO ADD NHIT FILTERING HERE TOO

	return tmp;
}

void make_timing_bitmap(int ring_num, hit_t hit_data, trigger_t *ptrigger)
{
  #include <iostream>
  using std::cout; using std::endl;
	ap_uint<4> t_buff=0;
	if(hit_data.t >=4)
		t_buff = hit_data.t; // map pre time 4 to 7 -> 4 to 7 (unchanged)
	else if(hit_data.t < 4)
		t_buff = hit_data.t + 8; // map cur time 0 to 3 -> 8 to 11 (move to time after pre hit window)
	ap_uint<3> t_actual = t_buff - 4;
  ptrigger->trig[ring_num][t_actual] = 1;

  cout << endl;
  cout << "ring_num: " << ring_num << endl;
  cout << "hit_data: " << hit_data.t << "\tt_buff: " << t_buff << "\tt_actual: " << t_actual << endl;
  cout << "ptrigger->trig[" << ring_num << "][" << t_actual 
       << "] = 1: " << ptrigger->trig[ring_num][t_actual] << endl;

  for(int i = 0; i < 8; i++){
    cout << "[" << ptrigger->trig[ring_num][i] << "]";
  }
  cout << "\n" <<  endl;
}