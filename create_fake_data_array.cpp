#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include <ap_int.h>

#include "variables.h"
#include "moller_hls.h"

void read_in_data(vector<hit_t> &vstorage, const string path)
{
	ifstream inp(path);
	if(inp)
	{
		int channel = 0 ;
		int energy = 0;
		int time = 0;
		while(inp >> channel >> energy >> time)
		{
			// cout << channel << ' ' << energy << ' ' << time << endl;
			vstorage.push_back({energy, time});
		}
	}
}


void create_data_array(const vector<hit_t> &vstorage, const string path)
{
	ofstream out(path);
	out << "#pragma once" << endl;
	out << "#include \"moller_hls.h\"" << endl;
	out << "#include \"variables.h\"" << endl;	

	
	int counter = 0;
	const int CHAN_COUNTER = N_CHAN;
	for(int run = 0; run < 3; run++){
		out << "const hit_t fake_data_" << run << "[N_CHAN] = \n{ ";
		for(int chan_index = 0; chan_index < N_CHAN; chan_index++){
			if(chan_index < N_CHAN-1)
				out << "{" << vstorage[chan_index+counter*CHAN_COUNTER].e << "," << vstorage[chan_index+counter*CHAN_COUNTER].t << "} , ";
			else
				out << "{" << vstorage[chan_index+counter*CHAN_COUNTER].e << "," << vstorage[chan_index+counter*CHAN_COUNTER].t << "} ";
		}
		counter++;
		out << "};" << endl;
		out << "\n" << endl;
	}
}

int main()
{
	vector<hit_t> vbuff;	
	hit_t arr_data[N_CHAN];
	// initilizing all elements to 0
	for(int chan_index = 0; chan_index < N_CHAN; chan_index++){
		arr_data[chan_index] = {0, 0};
	}
	read_in_data(vbuff,"data_stream.txt" );
	create_data_array(vbuff, "data_stream.h");

	return 0;
}
