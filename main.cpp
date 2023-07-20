// Branch Vitis

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using std::cout; using std::endl;

// for testing
#include <fstream>
#include <string>
#include "data_stream.h"

#include "moller_hls.h"

// channel mapping
#include "detector_type.h"
#include "chan_map.h"
#include <map>

void generateRndData(hit_t* vxs_chan){

	ap_uint<13> energy;
	ap_uint<3> time;
	for(int ch = 0; ch < N_CHAN; ch++){
		energy = 0;
		time = 0;
		if(ch%8 < 4){ // rings 1 - 4
           	if( (rand()%100) < 5 ){ // 5% chance
               	energy = rand() % 8192;
               	time = rand() % 8;
           	}
        }
        else if( (ch%8 < 7) && (ch%8 > 3) ){ // rings 5a, 5b, 5c
           	if( (rand()%100) < 10 ){ // 10% chance
               	energy = rand() % 8192;
               	time = rand() % 8;
           	}
        }
        else if( ch%8 > 6){ // ring 6
           	if( (rand()%100) < 5 ){ // 5% chance
               	energy = rand() % 8192;
               	time = rand() % 8;
           	}
        }
		vxs_chan[ch].e = energy;
		vxs_chan[ch].t = time;
	}
}


int main(int argc, char *argv[])
{
	// ap_uint<13> energy_threshold = 5e3;
	ap_uint<13> energy_threshold = 1;
	ap_uint<16> ring_threshold = 1;
	
	hls::stream<fadc_hits_t> s_fadc_hits; // raw data stream from the
	hls::stream<trigger_t> s_time_trigger; // output stream for for the trigger data
	hls::stream<ring_trigger_t> s_ring_trigger; // output stream for for the ring trigger data
	hls::stream<ring_all_t> s_ring_all; // output stream for the ring data
	hls::stream<ring_all_counter_t> s_ring_all_counter;

	char tmp;
	int ch;
	std::string detector_id;
	int seg_num;
	int sub_elem;

	fadc_hits_t fadc_hits;
	for(int ch = 0; ch < N_CHAN; ch++){
		fadc_hits.vxs_chan[ch].e = 0;
		fadc_hits.vxs_chan[ch].t = 0;
	}

	std::srand(1); // set specific seed for testing latency
// 	generateRndData(fadc_hits.vxs_chan);
	
	for(int i = 0 ; i <N_CHAN; i++){
		fadc_hits.vxs_chan[i].e = fake_data_0[i].e;
		fadc_hits.vxs_chan[i].t = fake_data_0[i].t;
	}	
	s_fadc_hits.write(fadc_hits);
	for(int i = 0 ; i <N_CHAN; i++){
		fadc_hits.vxs_chan[i].e = fake_data_1[i].e;
		fadc_hits.vxs_chan[i].t = fake_data_1[i].t;
	}	
	s_fadc_hits.write(fadc_hits);
	for(int i = 0 ; i <N_CHAN; i++){
		fadc_hits.vxs_chan[i].e = fake_data_2[i].e;
		fadc_hits.vxs_chan[i].t = fake_data_2[i].t;
	}	
	s_fadc_hits.write(fadc_hits);

	while(!s_fadc_hits.empty()){

		moller_hls
		(
			energy_threshold,
			ring_threshold,
			s_fadc_hits,
			s_time_trigger,
			s_ring_trigger,
	 		s_ring_all,
			s_ring_all_counter
		);

	}

	ofstream fout("C:/Users/mrcco/Documents/GitHub/MOLLER_VTP/vitis_only_rings_5abc_all_segs_no_scalars.txt");
	// TRIGGER INFO BLOCK
	printf("\nRing Data:__________________\n");
	while(!s_ring_all.empty())
	{
		ring_all_t ring_data = s_ring_all.read();
		for(int ring_index = 0; ring_index < 8; ring_index++){
			fout << "ringNum: " << ring_index << endl;
			fout << "ring_data.r[" << ring_index << "].e: " << ring_data.r[ring_index].e << endl;
			fout << "ring_data.r[" << ring_index << "].nhits: " << ring_data.r[ring_index].nhits << endl;
			fout << "ring_data.r[" << ring_index <<"].segment: ";
			for(int seg_index = 27; seg_index > -1; seg_index--){
				fout << "[" << ring_data.r[ring_index].segment[seg_index] << "]";
			}
			fout << endl;
		}
	}



	fout << "\nRing Trig Data:__________________\n";
	fout << "Format:\t 7 ------------------ 0\t ring #" << endl;
	while(!s_ring_trigger.empty())
	{
		ring_trigger_t ring_trig = s_ring_trigger.read();
		for(int ring_index = 7; ring_index > -1; ring_index--){
			fout << "[" << ring_trig.ring[ring_index] << "]";
		}
		fout << endl;
	}
	
	fout << "\n\nTime Trig Data:__________________\n";
	while(!s_time_trigger.empty())
	{
		int t32ns = 0;
		trigger_t time_trig = s_time_trigger.read();

		fout << "Format:\t 12ns ------------------ -16ns\t (4ns ticks)" << endl;
		for(int ring_index = 0; ring_index < 8; ring_index ++){
			for(int time_tick = 7; time_tick > -1; time_tick--){
				if(time_trig.trig[ring_index][time_tick]){
					fout << "Trigger found at T=" << t32ns*32+time_tick*4-16 << "ns" << endl;
				}
			}
			fout << "Ring: " << ring_index << "\t";
			for(int time_tick = 7; time_tick > -1; time_tick--){
				fout << "[" << time_trig.trig[ring_index][time_tick] << "]";
			}
			fout << endl;
		}
		t32ns++;
	}
	while(!s_ring_all_counter.empty())
	{
		ring_all_counter_t raw_counter = s_ring_all_counter.read();
		fout << "Raw Counter:" << endl;
		for(int i = 0; i < 8; i++){
			fout << "Ring " << i << ": " << raw_counter.ring_counter[i].counter << endl;
		}
	}
	
	return 0;
}
