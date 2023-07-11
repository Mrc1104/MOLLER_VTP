// Branch Vitis

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
#include "moller_hls.h"
#include "chan_map/array1.h"
#include "std_map.h"
#include "chan_map.h"
#include "detector_type.h"


void generateRndData(hit_t* vxs_chan, int seed){

	std::srand(seed); // set specific seed for testing latency
	ap_uint<13> energy;
	ap_uint<3> time;
	for(int ch = 0; ch < N_CHAN; ch++){
		energy = 0;
		time = 0;
		if(ch%8 < 4){ // rings 1 - 4
           	if( (rand()%100) < 10 ){ // 10% chance
               	energy = rand() % 8192;
               	time = rand() % 8;
           	}
        }
        else if( (ch%8 < 7) && (ch%8 > 3) ){ // rings 5a, 5b, 5c
           	if( (rand()%100) < 20 ){ // 20% chance
               	energy = rand() % 8192;
               	time = rand() % 8;
           	}
        }
        else if( ch%8 > 6){ // ring 6
           	if( (rand()%100) < 9 ){ // 10% chance
               	energy = rand() % 8192;
               	time = rand() % 8;
           	}
        }
		vxs_chan[ch].e = energy;
		vxs_chan[ch].t = time;
	}
}

struct vstruct
{
	int seg_number;
	ap_uint<13> energy;
	ap_uint<3> time;

};
int main()
{
	
	
	fadc_hits_t data;
	std::ofstream fout("data_stream.txt");
	std::ofstream f1("ring_one.txt");
	std::ofstream f2("ring_two.txt");
	std::ofstream f3("ring_three.txt");
	std::ofstream f4("ring_four.txt");
	std::ofstream f5("ring_five.txt");
	std::ofstream f6("ring_six.txt");
	std::ofstream f7("ring_seven.txt");
	std::ofstream f8("ring_eight.txt");
	
	for(int i = 0; i < 3; i++){
		vector<vstruct> ring_one;	
		vector<vstruct> ring_two;	
		vector<vstruct> ring_three;	
		vector<vstruct> ring_four;	
		vector<vstruct> ring_five;	
		vector<vstruct> ring_six;	
		vector<vstruct> ring_seven;	
		vector<vstruct> ring_eight;	
		generateRndData(data.vxs_chan, i);
		for(int ch = 0; ch < N_CHAN; ch++){
			fout << ch << '\t' << data.vxs_chan[ch].e << '\t' << data.vxs_chan[ch].t << endl;



			int ich = ch%16;
			int slot = (ch-ich)/16;
			det_type ring_num = chmap[slot][ich].DET_ID;
			int seg_num = chmap[slot][ich].SEG_NUM;
			int sub_elem = chmap[slot][ich].SUB_ELEMENT;
			switch(ring_num){
				case RING_ONE:
					ring_one.push_back({seg_num, data.vxs_chan[ch].e , data.vxs_chan[ch].t});
					break;
				case RING_TWO:
					ring_two.push_back({seg_num, data.vxs_chan[ch].e , data.vxs_chan[ch].t});
					break;
				case RING_THREE:
					ring_three.push_back({seg_num, data.vxs_chan[ch].e , data.vxs_chan[ch].t});
					break;
				case RING_FOUR:
					ring_four.push_back({seg_num, data.vxs_chan[ch].e , data.vxs_chan[ch].t});
					break;
				case RING_FIVE:
					if(sub_elem == 65) { ring_five.push_back({seg_num, data.vxs_chan[ch].e , data.vxs_chan[ch].t});  }
					if(sub_elem == 66) { ring_six.push_back({seg_num, data.vxs_chan[ch].e , data.vxs_chan[ch].t});   }
					if(sub_elem == 67) { ring_seven.push_back({seg_num, data.vxs_chan[ch].e , data.vxs_chan[ch].t}); }
					break;
				case RING_SIX:
					ring_eight.push_back({seg_num, data.vxs_chan[ch].e , data.vxs_chan[ch].t});
					break;
				default:
					cout << "DEFAULT CASE HIT.. THIS SHOULD NOT HAVE OCCURED" << endl;
					cout << "Check Entry: " << ch << " RING: " << ring_num << 
						" Seg: " << seg_num << " Sub: " << sub_elem << endl;
					break;
			}
		}
		fout << endl;



		f1 << "RUN: " << i << endl;
		f2 << "RUN: " << i << endl;
		f3 << "RUN: " << i << endl;
		f4 << "RUN: " << i << endl;
		f5 << "RUN: " << i << endl;
		f6 << "RUN: " << i << endl;
		f7 << "RUN: " << i << endl;
		f8 << "RUN: " << i << endl;
		for(auto it : ring_one){
			f1 << "Segment: " << it.seg_number << "\tEnergy: " << it.energy << "\tTime: " << it.time << endl;
		}
		for(auto it : ring_two){
			f2 << "Segment: " << it.seg_number << "\tEnergy: " << it.energy << "\tTime: " << it.time << endl;
		}
		for(auto it : ring_three){
			f3 << "Segment: " << it.seg_number << "\tEnergy: " << it.energy << "\tTime: " << it.time << endl;
		}
		for(auto it : ring_four){
			f4 << "Segment: " << it.seg_number << "\tEnergy: " << it.energy << "\tTime: " << it.time << endl;
		}
		for(auto it : ring_five){
			f5 << "Segment: " << it.seg_number << "\tEnergy: " << it.energy << "\tTime: " << it.time << endl;
		}
		for(auto it : ring_six){
			f6 << "Segment: " << it.seg_number << "\tEnergy: " << it.energy << "\tTime: " << it.time << endl;
		}
		for(auto it : ring_seven){
			f7 << "Segment: " << it.seg_number << "\tEnergy: " << it.energy << "\tTime: " << it.time << endl;
		}
		for(auto it : ring_eight){
			f8 << "Segment: " << it.seg_number << "\tEnergy: " << it.energy << "\tTime: " << it.time << endl;
		}
	}
}
