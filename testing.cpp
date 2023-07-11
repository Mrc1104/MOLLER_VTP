#include "iostream"
using namespace std;


#include "variables.h"
#include "moller_hls.h"

#include "data_stream.h"
int main()
{
	for(int run = 0 ; run < 3; run++){
		if(run == 0){
			for(int i = 0 ; i < N_CHAN; i++){
				cout << i << " " << fake_data_0[i].e << " " << fake_data_0[i].t << endl;
			}	
		}
		if(run == 1){
			for(int i = 0 ; i < N_CHAN; i++){
				cout << i << " " << fake_data_1[i].e << " " << fake_data_1[i].t << endl;
			}	
		}
		if(run == 2){
			for(int i = 0 ; i < N_CHAN; i++){
				cout << i << " " << fake_data_2[i].e << " " << fake_data_2[i].t << endl;
			}	
		}
		cout << endl;
	}


	return 0;
}
