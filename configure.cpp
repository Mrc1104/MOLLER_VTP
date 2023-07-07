#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

#include "detector_type.h"
#include "std_map.h"

using namespace std;

struct sdet
{
	string det;
	int seg;
	int sub;
};
void create(const string path);
void modify(const string path);

void print_header(ofstream& out);
vector<int> get_slot_numbers();
void get_channel_info(const vector<int> &slots, vector<sdet> &v, const int ii);
void get_channel_data(const vector<int> &slots, vector<sdet> &v, int chan_num);
void display_channel_choices();
int get_scint_sub_element(const vector<int> &v);


void read_contents(const string& file_name, vector<string> &lines);
void update_contents(vector<string> &lines);
void save_contents(const string& file_name, vector<string> &lines);
void find_avail_slots(const vector<string> &inp, vector<int> &out);
void string_parser(const string &inp, string &det, string &seg, string &sub);

int main(int argc, char* argv[])
{

	// Get the command line inputs 
	if(stoi(argv[1]) == 1) create(argv[2]);
	if(stoi(argv[1]) == 2) modify(argv[2]);

		


	return 0;
}



void create(const string path)
{
	ofstream out(path);
	print_header(out);
	vector<int> slots = get_slot_numbers();
	if(slots.empty()) {cout << "ERROR: No slots given... exiting\n"; return ;}
	cout << "HERE!" << endl;
	vector<sdet> vdata(16,{"NONE",0,0}); 
	for(int i = 0; i < slots.size(); i++){
		get_channel_info(slots, vdata, slots[i]);
		out << "\n# SLOT " << slots[i] << endl;
		for(int channel = 0; channel < vdata.size(); channel++){ // vdata.size() should == 16
			out << channel << "\t" << vdata[channel].det << "\t" << vdata[channel].seg << "\t" << vdata[channel].sub << endl;
		}
	}
	cout << "\n\n#########################################################################################\n";
	cout << "#########################################################################################\n";
	cout << "#########################################################################################\n";
	cout << "#########################################################################################\n";
	cout << "\nParameter File: (" << path << ") Created!" << endl; 
}
void modify(const string path)
{
	bool cont = true;
	vector<string> vbuffer;
	read_contents(path, vbuffer);
	do{
		update_contents(vbuffer);
		int intcont = 0;
		while(cout << "Continue Modifying? (n:0/y:1): " && !(cin >> intcont) || (intcont != 0 && intcont != 1)){
			cin.clear();
			cin.ignore();
			cout << "Invalid Input" << endl;
		}
		if(intcont == 1){
			cont = true;
		}
		else{
			cont = false;
		}
	}while(cont);
	save_contents(path,vbuffer);
}

void print_header(ofstream& out)
{
	
	string border = "#########################################################################################\n";
	vector<string> block = 
	{
		"# Description: Channel to detector mapping. Subdivided by the slot number of the        #\n",
		"#              VXS crate, each slot holds 16 (0 - 15) FADC channels that corresponds    #\n",
		"#              to a particular detector. Main.cpp will parse the information inserted   #\n",
		"#              below and create an array that will be used in the FPGA for arbritrary   #\n",
		"#              detector to channel mapping.                                             #\n",
		"#                                                                                       #\n",
		"# Note:        Any lines starting with '#' will be ignored. Delineate by spaces.        #\n",
		"#              FADC_CHAN_NUMBER - integer [0-15]                                        #\n",
		"#              DETECTOR_TYPE - enum (see detector_type.h for scheme)                    #\n",
		"#              SEGMENT_NUMBER - integer [0-27]                                          #\n",
		"#              SUB_ELEMENT - no sub-element => -1                                       #\n",
		"#                            for TRIG_SCINT, => chan # of TRIG_SCINT pair               #\n",
		"#                            for RING_5, => 65 (5a), 66 (5b), 67 (5c)                   #\n",
		"#                                                                                       #\n",
		"# FADC_CHAN_NUMBER,    DETECTOR_TYPE,  SEGMENT_NUMBER, SUB_ELEMENT                      #\n"
	};
	out << border;
	for(int i = 0; i < block.size(); i++)
	{
		out << block[i];
	}
	out << border;
}

vector<int> get_slot_numbers()
{
	int size = 0;
	cout << "#########################################################################################\n";
	while(cout << "Enter the Total Number of slots you will be working with: " && !(cin >> size)){
		cin.clear();
		cin.ignore();
		cout << "Invalid Input" << endl;
	}
	vector<int> v(size,-1);
	for(int i = 0; i < size; i++){
		int slot_num; 
		while(cout << "Slot #: " && !(cin >> slot_num)){
			cin.clear();
			cin.ignore();
			cout << "Invalid Input" << endl;
		}
		v[i] = slot_num;
	}
	sort( v.begin(), v.end() );
	return v;
}

void get_channel_info(const vector<int> &slots, vector<sdet> &v, const int ii)
{
	display_channel_choices();
	cout << "\n# SLOT " << ii << endl;
	for(int chan_num = 0; chan_num < 16; chan_num++){
		get_channel_data(slots, v, chan_num);
	}

	return;
}

void display_channel_choices()
{

	cout << "#########################################################################################\n";
	cout << "#########################################################################################\n";
	cout << "#########################################################################################\n";
	cout << "Options:\n";
	cout << "0) NONE\n";
	cout << "1) RING_ONE\n";
	cout << "2) RING_TWO\n";
	cout << "3) RING_THREE\n";
	cout << "4) RING_FOUR\n";
	cout << "5) RING_FIVE\tSub_elements: 3\n";
	cout << "6) RING_SIX\n";
	cout << "7) TRIG_SCINT\tSub_elements: 1\n";
	cout << "8) PION_DET\n";
	cout << "9) SHOWER_MAX\n";
	cout << "#########################################################################################\n";
	cout << "#########################################################################################\n";
	cout << "Enter 0 - 9:\n";
}

int get_scint_sub_element(const vector<int> &v)
{
	int sub_slot;
	int sub_chan;
	cout << "Sub_element:\n";
	cout << "Enter the SLOT # of the TRIG_SCINT PAIR: ";
	while( !(cin >> sub_slot)  || ( find(v.begin(),v.end(), sub_slot) == v.end() ) ){ // Check to see if the input is contained in v
		cin.clear();
		cin.ignore();
		cout << "Invalid Input! Possible slot choices: ";
		for(int index = 0; index < v.size(); index++){
			cout <<	v[index] << " "; 
		}
		cout << endl;
	}
	cout << "Enter the Channel # of the TRIG_SCINT PAIR: ";
	while( !(cin >> sub_chan)  && (sub_chan > -1 && sub_chan < 16 ) ){ // logic does not work, why?
		cin.clear();
		cin.ignore();
		cout << "Invalid Input! Enter a value between 0 - 15: ";
	}

	int index_sub_slot = distance(v.begin(), find(v.begin(), v.end(), sub_slot) );
	int absolute_channel = index_sub_slot * 16 + sub_chan;
	// cout << "index_sub_slot: " << index_sub_slot << endl;
	// cout << "absolute_channel " << absolute_channel << endl;
	return absolute_channel;
}


void read_contents(const string& file_name, vector<string> &lines)
{
	ifstream inp(file_name);
	string tmp;
	while(getline(inp,tmp)){
		lines.push_back(tmp);
	}
}


void update_contents(vector<string> &lines)
{
	vector<int> avail_slots;
	find_avail_slots(lines, avail_slots);
	cout << "Available Slots:\n";
	for(int i=0; i < avail_slots.size(); i++){
		cout << "SLOT #: " << avail_slots[i] << endl;	
	}
	cout << "Which one would you like to view?\nEnter SLOT #: ";
	int choice = 0;
	while( !(cin >> choice) || find(avail_slots.begin(),avail_slots.end(), choice) == avail_slots.end() ){
		cin.clear();
		cin.ignore();
		cout << "Invalid Input. Please re-Enter: "; 	
	}

	string key = "# SLOT " + to_string(choice);
	vector<string>::iterator it = lines.begin();
	while( *it != key ){
		it++;
	}
	it++;
	vector<string>::iterator it_sec = it;

	vector<sdet> vchanges;
	cout << "#########################################################################################\n";
	for(int counter = 0; counter < 16; counter++){
		string tmp = *it;
		string det_tmp = "";
		string seg_tmp = "";
		string sub_tmp = "";
		if(!tmp.empty()){
			string_parser(tmp, det_tmp, seg_tmp, sub_tmp);
		}
		int seg_int = stoi(seg_tmp);
		int sub_int = stoi(sub_tmp);
		sdet store = {det_tmp, seg_int, sub_int};
		vchanges.push_back(store);
		cout << counter << ") " << det_tmp << " " << seg_int << " " << sub_int << " " << endl;
		it++;
	}
	cout << "-1) Finished" << endl;
	cout << "_________________________________________________________________________________________\n";

	int cchannels = 0;
	vector<int> channels_to_change; 
	while(cchannels != -1){	
		while(cout << "Which channel would you like to change? " && !(cin>>cchannels) && (cchannels < 16 && cchannels > -2)){
			cin.clear();
			cin.ignore();
			cout << "Invalid Input" << endl;
		}
		if(cchannels != -1)
			channels_to_change.push_back(cchannels);
	}
	display_channel_choices();
	for(auto it : channels_to_change){
		get_channel_data(avail_slots,vchanges,it);
	}

	vector<string> vchanges_but_strings;
	for(int in = 0; in < vchanges.size(); in++){
		string st = to_string(in)+"\t"+vchanges[in].det+"\t"+to_string(vchanges[in].seg)+"\t"+to_string(vchanges[in].sub);
		vchanges_but_strings.push_back(st);
	}
	cout << "_________________________________________________________________________________________\n";
	vector<string>::iterator it_tmp = it_sec;
	for(int i = 0; i < vchanges_but_strings.size();i++){
		if(vchanges_but_strings[i] == *it_tmp){
			cout << *it_tmp << endl;
		}
		else{
			cout << *it_tmp << "\t->\t" << vchanges_but_strings[i] << endl;
		}
		it_tmp++;
	}
	cout << "_________________________________________________________________________________________\n";
	int keep_changes= 0;
	while(cout << "Keep Changes? (n:0/y:1): " && !(cin >> keep_changes) || (keep_changes != 0 && keep_changes != 1)){
		cin.clear();
		cin.ignore();
		cout << "Invalid Input" << endl;
	}
	if(keep_changes == 1){
		for(int i = 0; i < vchanges_but_strings.size(); i++){
			*it_sec = vchanges_but_strings[i];
			it_sec++;
		}
	}
	else
		cout << "Exiting... No Changes Saved" << endl;
}

void find_avail_slots(const vector<string> &inp, vector<int> &out)
{
	for(int i = 0; i < inp.size(); i++){
		if(inp[i][0] == '#' && inp[i].substr(2,4) == "SLOT"){
			string tmp = inp[i].substr(inp[i].size()-2,2);
			int t = stoi(tmp);
			out.push_back(t);
		}
	}

}


void get_channel_data(const vector<int> &slots, vector<sdet> &v, int chan_num)
{
	int det_choice = -1;
	int seg_choice = 0;
	int sub_choice = 0;
	while( ( cout << "CH:" << chan_num << " " && !(cin >> det_choice) ) && (det_choice > -1 && det_choice < 10) ){ // this logic does not work... why?
		cin.clear();
		cin.ignore();
		cout << "Invalid Input! Enter 0 - 9: ";
	}
	while( ( cout << "Segment #: " && !(cin >> seg_choice) ) && (seg_choice > -1 && seg_choice < 27) ){ // this logic does not work... why?
		cin.clear();
		cin.ignore();
		cout << "Invalid Input! Enter 0 - 27: ";
	}

	switch(det_choice)
	{
		case 1:
			v[chan_num].det = "RING_ONE";
			v[chan_num].seg = seg_choice;
			v[chan_num].sub = -1;
			break;
		case 2:
			v[chan_num].det = "RING_TWO";
			v[chan_num].seg = seg_choice;
			v[chan_num].sub = -1;
			break;
		case 3:
			v[chan_num].det = "RING_THREE";
			v[chan_num].seg = seg_choice;
			v[chan_num].sub = -1;
			break;
		case 4:
			v[chan_num].det = "RING_FOUR";
			v[chan_num].seg = seg_choice;
			v[chan_num].sub = -1;
			break;
		case 5:
			v[chan_num].det = "RING_FIVE";
			v[chan_num].seg = seg_choice;
			cout << "_________________________________________________________________________________________\n";
			cout << "1) RING 5A\n";
			cout << "2) RING 5B\n";
			cout << "3) RING 5C\n";
			cout << "Sub_element: ";
			while( !(cin >> sub_choice)  && (sub_choice > 0 && sub_choice < 4) ){ // this logic does not work... why?
				cin.clear();
				cin.ignore();
				cout << "Invalid Input! Enter 1 - 3: ";
			}
			cout << "_________________________________________________________________________________________\n";
			v[chan_num].sub = sub_choice + 64;
			break;
		case 6:
			v[chan_num].det = "RING_SIX";
			v[chan_num].seg = seg_choice;
			v[chan_num].sub = -1;
			break;
		case 7:
			v[chan_num].det = "TRIG_SCINT";
			v[chan_num].seg = seg_choice;
			v[chan_num].sub = -1;
			cout << "_________________________________________________________________________________________\n";
			v[chan_num].sub = get_scint_sub_element(slots);
			cout << "_________________________________________________________________________________________\n";
			break;
		case 8:
			v[chan_num].det = "PION_DET";
			v[chan_num].seg = seg_choice;
			v[chan_num].sub = -1;
			break;
		case 9:
			v[chan_num].det = "SHOWER_MAX";
			v[chan_num].seg = seg_choice;
			v[chan_num].sub = -1;
			break;
		default:
			v[chan_num].det = "NONE";
			v[chan_num].seg = seg_choice;
			v[chan_num].sub = -1;
			break;
	}

}

void string_parser(const string &inp, string &det, string &seg, string &sub)
{

	string tmp = "";
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
			if(space_counter == 1){
				det+=inp[i];
			}
			if(space_counter == 2){
				seg+=inp[i];
			}
			if(space_counter == 3){
				sub+=inp[i];
			}
		}
	}
}


void save_contents(const string& file_name, vector<string> &lines)
{
	ofstream out(file_name);
	for(auto it : lines){
		out << it << endl;
	}

	cout << "_________________________________________________________________________________________\n";
	cout << "Changes Written to " << file_name << endl;

}
