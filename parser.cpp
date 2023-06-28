#include <iostream>
#include <fstream>
#include <string>
#include "chan_map.h"
#include "variables.h"
#include "std_map.h"
using namespace std;


void read_para_file(string sFile, chan_map chmap[][16])
{
	ifstream fchan_map(sFile);	
	char tmp;
	int ch;
	std::string detector_id, comment;
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
				cout << ch << detector_id << seg_num << sub_elem;
				chmap[slot][ch] = {m.at(detector_id), seg_num, sub_elem};
				// cout << channelCount << " chmap[" << slot <<"]["<<ch<<"] = " << m.at(detector_id) << " " << seg_num << " " << sub_elem << endl;
				channelCount++;
				if( channelCount%16 == 0) slot++;
			}
		}
	}

}

void save_chan_map_array(chan_map arr[][16], std::string path)
{
	std::ofstream fout_array(path.c_str());
	fout_array << "#pragma once\n";
	fout_array << "#include \"../moller_hls.h\"\n";
	fout_array << "#include \"../chan_map.h\"\n\n";
	fout_array << "const chan_map chmap[N_SLOT][16] = {";
	cout << "#pragma once\n";
	cout << "#include \"../moller_hls.h\"\n";
	cout << "#include \"../chan_map.h\"\n\n";
	cout << "const chan_map chmap[N_SLOT][16] = {";
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

int main(int argc, char* argv[])
{
	std::string sFile;
	std::string sHeader;

	// Get the command line inputs 
	sFile = argv[1];	
	sHeader = argv[2];	

	std::cout << "sFile: " << sFile << " " << "sHeader :" << sHeader << endl;
	chan_map chmap[N_SLOT][16]; // N_SLOT is defined in moller_hls.h
	read_para_file(sFile, chmap);
	save_chan_map_array(chmap, sFile);

	return 0;
}

