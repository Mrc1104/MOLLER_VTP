#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

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
vector<sdet> get_channel_info(const vector<int> &slots, const int ii);
void display_channel_choices();
int get_scint_sub_element(const int curr_slot, const int curr_chan, const vector<int> &v);



int main(int argc, char* argv[])
{

	// Get the command line inputs 
	if(stoi(argv[1]) == 1) create(argv[2]);
	if(stoi(argv[1]) == 2) modify(argv[2]);

		


	return 1;
}



void create(const string path)
{
	ofstream out(path);
	print_header(out);
	vector<int> slots = get_slot_numbers();
	if(slots.empty()) {cout << "ERROR: No slots given... exiting"; return ;}
	for(int i = 0; i < slots.size(); i++){
		vector<sdet> vdata = get_channel_info(slots, slots[i]);
		out << "\n# SLOT " << slots[i] << endl;
		for(int channel = 0; channel < vdata.size(); channel++){ // vdata.size() should == 16
			cout << "ch: " << vdata.size();	
			out << channel << "\t" << vdata[channel].det << "\t" << vdata[channel].seg << "\t" << vdata[channel].sub << endl;
		}
	}
}
void modify(const string path)
{


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
		"#                            for TRIG_SCINT, => chan # if TRIG_SCINT pair               #\n",
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

vector<sdet> get_channel_info(const vector<int> &slots, const int ii)
{
	vector<sdet> v(16,{"NONE",0,0}); 
	int det_choice = 0;
	int seg_choice = 0;
	int sub_choice = 0;
	display_channel_choices();
	cout << "\n# SLOT " << ii << endl;
	for(int chan_num = 0; chan_num < 16; chan_num++){
	
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
			break;
		case 2:
			v[chan_num].det = "RING_TWO";
			v[chan_num].seg = seg_choice;
			break;
		case 3:
			v[chan_num].det = "RING_THREE";
			v[chan_num].seg = seg_choice;
			break;
		case 4:
			v[chan_num].det = "RING_FOUR";
			v[chan_num].seg = seg_choice;
			break;
		case 5:
			v[chan_num].det = "RING_FIVE";
			v[chan_num].seg = seg_choice;
			cout << "Sub_element:\n";
			cout << "1) RING 5A\n";
			cout << "2) RING 5B\n";
			cout << "3) RING 5C\n";
			while( !(cin >> sub_choice)  && (sub_choice > 0 && sub_choice < 4) ){ // this logic does not work... why?
				cin.clear();
				cin.ignore();
				cout << "Invalid Input! Enter 1 - 3: ";
			}
			v[chan_num].sub = sub_choice + 64;
			break;
		case 6:
			v[chan_num].det = "RING_SIX";
			v[chan_num].seg = seg_choice;
			break;
		case 7:
			v[chan_num].det = "TRIG_SCINT";
			v[chan_num].seg = seg_choice;
			v[chan_num].sub = get_scint_sub_element(ii, chan_num, slots);
			break;
		case 8:
			v[chan_num].det = "PION_DET";
			v[chan_num].seg = seg_choice;
			break;
		case 9:
			v[chan_num].det = "SHOWER_MAX";
			v[chan_num].seg = seg_choice;
			break;
		default:
			v[chan_num].det = "NONE";
			v[chan_num].seg = seg_choice;
			break;
		}
	}

	return v;
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

int get_scint_sub_element(const int curr_slot, const int curr_chan, const vector<int> &v)
{
	int sub_choice;
	int sub_slot;
	int sub_chan;
	cout << "Sub_element:\n";
	cout << "Enter the SLOT # of the TRIG_SCINT PAIR: \nTRIG_SCINT PAIR SLOT#: ";
	while( !(cin >> sub_slot)  && ( find(v.begin(),v.end(), sub_slot) != v.end() ) ){ // Check to see if the input is contained in v
		cin.clear();
		cin.ignore();
		cout << "Invalid Input! Possible slot choices: ";
		for(int index = 0; index < v.size(); index++){
			cout <<	v[index] << " "; 
		}
		cout << endl;
	}
	cout << "Enter the Channel # of the TRIG_SCINT PAIR: \nTRIG_SCINT PAIR CHAN #: ";
	while( !(cin >> sub_chan)  && (sub_chan > -1 && sub_chan < 16 ) ){ // logic does not work, why?
		cin.clear();
		cin.ignore();
		cout << "Invalid Input! Enter a value between 0 - 15: ";
	}
	/* time to do math
	 * 0) Cast all slot #s into vector indices
	 * 1) Figure out which slot is larger	
	 * 2) How many channels are left in smaller slot?
	 * 3) How many empty slots between small and larger slot?
	 * 4) How many channels in larger slot?
	 * 5) Calculate absolute channel number of TRIG_SCINT PAIR
	 * 		5a) we need the absolute index of the smaller index
	*/
	int larger_slot = 0;
	int smaller_slot = 0;
	int chan_remaining_smaller_slot = 0;
	int chan_larger_slot = 0;

	int index_curr_slot = find(v.begin(), v.end(), curr_slot) - v.begin();
	int index_sub_slot = find(v.begin(), v.end(), sub_slot) - v.begin();
	if( (index_curr_slot - index_sub_slot) > 0) { 
		larger_slot = index_curr_slot;
		smaller_slot = index_sub_slot;
		chan_larger_slot = curr_slot;
		chan_remaining_smaller_slot = 15 - sub_chan;
	
	}
	else {
		larger_slot = index_sub_slot;
		smaller_slot = index_curr_slot;
		chan_larger_slot = sub_slot;
		chan_remaining_smaller_slot = 15 - curr_chan;
	}
	int slot_between = 0;
	for(int tmp_index = smaller_slot; tmp_index < larger_slot-1; tmp_index++, slot_between++);
	int chan_between = slot_between * 16;

	int absolute_channel =  chan_remaining_smaller_slot + (slot_between*16) + chan_larger_slot + (smaller_slot)*16 + (16-chan_remaining_smaller_slot);
	cout << "absolute_channel " << absolute_channel << endl;
	return sub_choice;
}
