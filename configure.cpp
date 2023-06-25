#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


void create(const string path);
void modify(const string path);

void print_header(ofstream& out);
vector<int> get_slot_numbers();

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
	return v;
}
