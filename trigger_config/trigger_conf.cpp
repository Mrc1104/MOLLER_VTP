#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
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
			r.push_back(ring_counter);
			scal.push_back(stoi(scalar));
			seg.push_back(vdummy);
		}
	}
}

ap_uint<8> make_ring_bitmap(const vector<int> r)
{
	ap_uint<8> ring_bitmap = 0;


	return ring_bitmap;
}

ap_uint<28> make_segment_bitmap(const vector<int> s)
{
	ap_uint<28> seg_bitmap = 0;

	return seg_bitmap;
}

void create_header(ofstream &out)
{

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

	
	// ap_uint<8> make_ring_bitmap(const vector<int> r)
	// ap_uint<28> make_segment_bitmap(const vector<int> s)


	return 0;
}
