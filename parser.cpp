#include <iostream>
#include <fstream>
#include <string>
#include "chan_map.h"

void save_chan_map_array(chan_map arr[][16], std::string path)
{
	std::ofstream fout_array(path.c_str());
	fout_array << "#pragma once\n";
	fout_array << "#include \"../moller_hls.h\"\n";
	fout_array << "#include \"../chan_map.h\"\n\n";
	fout_array << "chan_map chmap[N_SLOT][16] = {";
	for(int i = 0 ; i < N_SLOT; i++){
		fout_array << "\n{ ";
		for(int j = 0; j < 16; j++){
			if(j < 15)
				fout_array << "{" << m2.at(arr[i][j].DET_ID) << ", " << arr[i][j].SEG_NUM << ", " << arr[i][j].SUB_ELEMENT << "},";
			else
				fout_array << "{" << m2.at(arr[i][j].DET_ID) << ", " << arr[i][j].SEG_NUM << ", " << arr[i][j].SUB_ELEMENT << "}";
		}
		if(i < N_SLOT-1)
			fout_array << " },";
		else
			fout_array << " }\n";
	}
	fout_array << "};";

}

int main()
{

	chan_map chmap[N_SLOT][16]; // N_SLOT is defined in moller_hls.h


	return 0;
}
