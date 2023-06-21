#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using std::cout; using std::endl;

// for testing
#include <fstream>
#include <string>

#include "moller_hls.h"

// channel mapping
#include "detector_type.h"
#include "chan_map.h"
#include <map>

void generateRndData(hit_t* vxs_chan){

	std::srand(std::time(0)); // set random seed
	ap_uint<13> energy;
	ap_uint<3> time;
	for(int ch = 0; ch < N_CHAN_SEC; ch++){
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
		vxs_chan[ch].t = energy;
	}
}


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

	// I/O BLOCk
	std::string comment;

	// load channel to det mapping
	std::ifstream fchan_map;
	fchan_map.open("chan_map/det_map_naive.dat");
	chan_map chmap[N_SLOT][16]; // N_SLOT is defined in moller_hls.h
								// Each FADC has 16 chans (one FADC per slot)
	char tmp;
	int ch;
	std::string detector_id;
	int seg_num;
	int sub_elem;

	int slot = 0;

	int channelCount = 0;
	if(fchan_map){
		while( fchan_map >> tmp){
			if(tmp == '#'){ // ignore # lines
				std::getline(fchan_map, comment);
			}
			else{
				fchan_map.putback(tmp); // char has issues reading multi-digit #s 
				fchan_map >> ch >> detector_id >> seg_num >> sub_elem;
				chmap[slot][ch] = {m[detector_id], seg_num, sub_elem};
				cout << channelCount << " chmap[" << slot <<"]["<<ch<<"] = " << m[detector_id] << " " << seg_num << " " << sub_elem << endl;
				// channelCount++;
				if(ch == 15) slot++;
			}
		}
	}
	std::ofstream fout_array("chan_map/array1.h");
	fout_array << "chan_map chmap[N_SLOT][16] = {";
	for(int i = 0 ; i < N_SLOT; i++){
		fout_array << "\n{ ";
		for(int j = 0; j < 16; j++){
			if(j < 15)
				fout_array << "{" << chmap[i][j].DET_ID << ", " << chmap[i][j].SEG_NUM << ", " << chmap[i][j].SUB_ELEMENT << "},";
			else
				fout_array << "{" << chmap[i][j].DET_ID << ", " << chmap[i][j].SEG_NUM << ", " << chmap[i][j].SUB_ELEMENT << "}";
		}
		if(i < N_SLOT-1)
			fout_array << "},";
		else
			fout_array << "}\n";
	}
	fout_array << "};";


	/*
	// load test data
	std::ifstream testData;
	testData.open("test_data/fake_timing_data.txt");

	fadc_hits_t fadc_hits;
	for(int ch = 0; ch < N_CHAN_SEC; ch++){
		fadc_hits.vxs_chan[ch].e = 0;
		fadc_hits.vxs_chan[ch].t = 0;
	}
	// generateRndData(fadc_hits.vxs_chan);
	if(testData){
		std::getline(testData, comment);
		int index;
		ap_uint<13> en = 0;
		ap_uint<3> ti = 0;
		while( testData >> index  ){
			if(index < 0 || index > 223){
				cout << "Trying to read into a nonexistant fadc channel" << endl;
				break; // ran out of fadc channels
			}
			testData >> en >> ti;
			std::cout << index << " " << en << " "
				 	  << ti << " " << std::endl;
			fadc_hits.vxs_chan[index].e = en;
			fadc_hits.vxs_chan[index].t = ti;
		}
	}
	else{
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
	
	// TRIGGER INFO BLOCK
	printf("\nRing Data:__________________\n");
	while(!s_ring_all_t.empty())
	{
		ring_all_t ring_data = s_ring_all_t.read();
		for(int ring_index = 0; ring_index < 8; ring_index++){
			cout << "ringNum: " << ring_index << endl;
			cout << "ring_data.r[" << ring_index << "].e: " << ring_data.r[ring_index].e << endl;
			cout << "ring_data.r[" << ring_index << "].nhits: " << ring_data.r[ring_index].nhits << endl;
			// cout << "ring_data.r[" << i << "].segment: " << ring_data.r[i].segment << endl;
			cout << "ring_data.r[" << ring_index <<"].segment: ";
			for(int seg_index = 27; seg_index > -1; seg_index--){
				cout << "[" << ring_data.r[ring_index].segment[seg_index] << "]";
			}
			cout << endl;
		}
	}

	printf("\nRing Trig Data:__________________\n");
	cout << "Format:\t 7 ------------------ 0\t ring #" << endl;
	while(!s_ring_trigger.empty())
	{
		ring_trigger_t ring_trig = s_ring_trigger.read();
		for(int ring_index = 7; ring_index > -1; ring_index--){
			cout << "[" << ring_trig.ring[ring_index] << "]";
		}
	}
	
	printf("\n\nTime Trig Data:__________________\n");
	while(!s_time_trigger.empty())
	{
		int t32ns = 0;
		trigger_t time_trig = s_time_trigger.read();

		cout << "Format:\t 7 ------------------ 0\t 4ns ticks" << endl;
		for(int ring_index = 0; ring_index < 8; ring_index ++){
			for(int time_tick = 7; time_tick > -1; time_tick--){
				if(time_trig.trig[ring_index][time_tick]){
					printf("Trigger found at T=%dns\n", t32ns*32+time_tick*4-16);
				}
			}
			cout << "Ring: " << ring_index << "\t";
			for(int time_tick = 7; time_tick > -1; time_tick--){
				cout << "[" << time_trig.trig[ring_index][time_tick] << "]";
			}
			cout << endl;
		}
		t32ns++;
	}

*/
	return 0;
}
