#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include <ap_int.h>
using namespace std;

bool save_file_already_exists(const string path)
{
	ifstream test(path);
	if(test)
		return true;
	else
		return false;
}

void parse_string(const string &inp, string &ON_OFF, string &scalar, string &segments)
{
	bool prevSpace= false;
	int space_counter = 0;
	for(int i = 0 ; i < inp.size(); i++){
		if(inp[i]==' ' || inp[i]== '\t'){
			prevSpace = true;
			continue;
		}	
		else{
			if(prevSpace){
				prevSpace = false;
				space_counter++;
			}
			if(space_counter == 0){
				ON_OFF+=inp[i];
			}
			if(space_counter == 1){
				scalar+=inp[i];
			}
			if(space_counter == 2){
				if(inp[i] == '[' || inp[i] == ',' || inp[i] == ']' || inp[i] == ' '){
					if(segments.size() > 0)
						segments += ' ';
					continue;
				}
				segments += inp[i];
			}
		}
	} 
}

void parser(ifstream &inp, vector<int> &r, vector<vector<int>> &seg, vector<int> &scal)
{
	char tmp;
	string comment;
	int ring_counter = 0;
	while(inp >> tmp){
		string on_off = "";
		string segment = "";
		string scalar = "";
		if(tmp == '#'){ // ignore # lines
			getline(inp, comment);
			continue;
		}
		else{
			inp.putback(tmp);
			getline(inp, comment);
			parse_string(comment, on_off, scalar, segment);	
			ring_counter++;
		}
		if(on_off == "ON"){
			vector<int> vdummy;
			string sdummy = "";
			for(int index = 0; index < segment.size(); index++){
				if(segment[index] != ' ')
					sdummy += segment[index];
				else{
					int idummy = stoi(sdummy);
					vdummy.push_back(idummy);
					sdummy = "";
				}
			}
			r.push_back(ring_counter-1);
			scal.push_back(stoi(scalar));
			seg.push_back(vdummy);
		}
	}
}

ap_uint<8> make_ring_bitmap(const vector<int> r)
{
	ap_uint<8> ring_bitmap = 0;
	for(auto it : r)
	{
		ring_bitmap[it] = 1;
	}	
	return ring_bitmap;
}

ap_uint<28> make_segment_bitmap(const vector<int> s)
{
	ap_uint<28> seg_bitmap = 0;
	if( find(s.begin(),s.end(), 0) == s.end() && s.size() != 0 ){
		if(find(s.begin(), s.end(), 29) == s.end()){
			for(auto it : s){
				seg_bitmap[it-1] = 1;
			}			
		}
		else
		{
			for(int i = 0 ; i < 28; i++)
			{
				seg_bitmap[i] = 1;
			}
		}
	}
	return seg_bitmap;
}

void create_header(ofstream &out , const ap_uint<8> r, const ap_uint<28> *s, const ap_uint<8> *scal)
{
	out << "#pragma once\n";
	out << "#include <ap_int.h>\n";
	out << endl;
	out << "const ap_uint<8> ring_trigger_config_bitmap = " << r << ";\n// ";
	for(int i = 7; i>-1; i--){
		out << "[" << r[i] << "]";
	}
	out << "\n// MSB ----------------- LSB" << endl;
	out << endl;
	out << "const ap_uint<28> segment_trigger_config_bitmap[8] = \n{\n";
	for(int i = 0; i < 8; i++){
		out << "// ";
		for(int j = 27; j > -1; j--){
			out << "[" << s[i][j] << "]";
		}
		out << endl;
		if(i < 7){
			out << s[i] << ",";
			if(i == 0) out << " // Ring 1";
			out << "\n";
		}
		else
			out << s[i] << " // Ring 8\n};";
	}
	out << endl;
	out << "const ap_uint<8> ring_trigger_scalars[8] =\n{\n";
	for(int i = 0 ; i < 8; i++){
		if(i < 7){
			out << " " << scal[i] << ",";
			if(i == 0){
				out << " // Ring 1";
			}
			out << endl; 
		}	
		else{
			out << " " << scal[i] << "  // Ring 8\n};";
		}
	}

}

int main(int argc, char **argv)
{
	if(argc < 3){
		cout << "\nRun the program with the additonal arguments:" << endl;
		cout << "\t1) path/to/trigger/conf/file\n";
		cout << "\t2) path/to/save/location\n";
		cout << "ie: ./trigger_setup ~/path/trigger.conf ~/path/save/file\n" << endl;
	}	
	
	string config_file_path = argv[1];
	string config_save_path = argv[2];
	if(save_file_already_exists(config_save_path))
	{
		bool overwrite = 0;
		cout << "Save File (" << config_save_path << ") already exists!\n";
		while(cout << "Overwrite? (n:0/y:1): " && !(cin >> overwrite) || (overwrite !=0 && overwrite !=1))
		{
			cin.clear();
			cin.ignore();
			cout << "Invalid Input" << endl;
		}
		if(!overwrite){
			cout << "Aborting...\n";	
			return 1;
		}
	}

	ifstream config_file(config_file_path); 
	if(!config_file){
		cout << "Configuration file (" << config_file_path << ") does not exist!" << endl;
		cout << "Aborting...\n";
		return 1;
	}
	ofstream save_file(config_save_path);
	
	cout << "_________________________________________________________________________________________\n";
	cout << "Opening Configuration file: " << config_file_path << endl;
	cout << "Saving to file: " << config_save_path << endl;
	cout << "_________________________________________________________________________________________\n";

	vector<int> rings;
	vector<vector<int>> segments;
	vector<int> scalars;
	parser(config_file, rings, segments, scalars);

	
	ap_uint<8> ring_bitmap = make_ring_bitmap(rings);



	ap_uint<28> segment_bitmap;
	ap_uint<28> arr_segment_bitmap[8];
	for(int i = 0 ; i < 8; i++){
		arr_segment_bitmap[i] = 0;
	}
	for(int i = 0 ; i < segments.size(); i++){
		segment_bitmap = make_segment_bitmap(segments[i]);
		arr_segment_bitmap[rings[i]] = segment_bitmap;	
	}
	
	
	ap_uint<8> scalar_array[8]; 
	for(int index = 0; index < 8; index++){
		scalar_array[index] = 0 ;		
	}
	for(int r_index = 0; r_index<rings.size(); r_index++){
		scalar_array[rings[r_index]] = scalars[r_index];	
	}
	create_header(save_file, ring_bitmap, arr_segment_bitmap, scalar_array);
	return 0;
}
