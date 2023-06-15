#include <iostream>
#include <fstream>
#include <random>
#include <ap_int.h>
#include <hls_stream.h>

#include "moller_hls.h"
#include "moller_hls.cpp"

using namespace std;

int main()
{
    ofstream out("test_data/random_fake_data");
    out << "# chan energy time";


    srand(time(0));
    ap_uint<13> e;
    ap_uint<3> t;
    for(int ch; ch < N_CHAN_SEC; ch++){
        if(ch%8 < 4){ // rings 1 - 4
            if( (rand()%100) < 10 ){
                e = rand() % 8192;
                t = rand() % 8;
            }
        }
        else if( (ch%8 < 7) && (ch%8 > 3) ){ // rings 5a, 5b, 5c
            if( (rand()%100) < 40 ){
                e = rand() % 8192;
                t = rand() % 8;
            }
        }
        else if( ch%8 > 6){ // ring 6
            if( (rand()%100) < 10 ){
                e = rand() % 8192;
                t = rand() % 8;
            }
        }
        else{
            e = 0;
            t = 0;
        }
    
        out << e << t;
    }




    return 0;
}