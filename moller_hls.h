#ifndef MOLLER_HLS_H
#define MOLLER_HLS_H

#include <ap_int.h>
#include <hls_stream.h>


// hit_t:
// - every 32ns each fadc reports 13 bit energy, and 3 bit hit time (time offset in current 32ns clock: 0=0ns, 1=4ns, 2=8ns, ..., 7=28ns)
// - if the channel has no hit, then the energy, e, will be reported as 0
// - energy, e, will saturate at 8191 (e.g. if the FADC intergal [after pedastal subtr. and gian] is greater than 8191, the FADC reports 8191)
typedef struct 
{
	ap_uint<13> e;
	ap_uint<3> t;
} hit_t;

// fadc_hits_t:
// - contains 244 VXS channels of hit_t reported by the FADCs each 32ns pulse
// - vxs_ch[ 0 ] to vxs_ch[ 15 ]: VME slot 4, ch 0 to 15 FADC channels
// - vxs_ch[ 16 ] to vxs_ch[ 31 ]: VME slot 5, ch 0 to 15 FADC channels
// - vxs_ch[ 112 ] to vxs_ch[ 127 ]: VME slot 5, ch 0 to 15 FADC channels
// ...
// - (VXS switch A and B are at VME slot positions 11,12 so the FADC cannot be installed here
// - vxs_ch[ 144 ] to vxs_ch[ 159 ]: VME slot 5, ch 0 to 15 FADC channels
// ...
// - vxs_ch[ 208 ] to vxs_ch[ 224 ]: VME slot 19, ch 0 to 15 FADC channels
//
//#define N_CHAN_SEC 64; // number of FADC channels per sector (16 FADC ch per segment, 4 segment per sector)
#define N_CHAN_SEC 224; // number of FADC channels per sector (16 FADC ch per segment, 4 segment per sector)
// Be careful, max array n can make is 4096 bits wide (4096/16 = 256 so we are good)
typedef struct
{
	hit_t vxs_chan[N_CHAN_SEC];
} fadc_hits_t;


// ring_hit_t:
// - Hit data for a single ring
// - e: sum of the energy of all the hits on a ind. ring
// - t: ring time measured at the central hit time
// - nhits: number of hits on an ind. ring
// - bbuff: nit buffer to make struct multiple of 8bits
typedef struct
{
	ap_uint<16> e;
	ap_uint<3> t;
	ap_uint<3> nhits;
	ap_uint<2> bbuff;
} ring_hit_t;

// ring_loc_t:
// - This is the general data structure to store the information about hits on a ring;
//   which segment / sector the hit is in and the timing.
// - trig: [0]=>0ns, [1]=>4ns, [2]=>8ns, ..., [7]=28ns, when bit=0 no trigger, when bit=1 trigger
// - segment: bit map for the four segments per sector. bit=0, no hit; bit=1, hit
// - sector: bit map for the seven sectors. bit=0, no hit; bit = 1 hit
// - bbuf: bit buffer to make structs multiples of 8bits
typedef struct
{
	ap_uint<8> trig;
	ap_uint<4> segment;
	ap_uint<7> sector;
	ap_uint<5> bbuf;
} ring_loc_t;

typedef struct
{
	ring_loc_t loc;
	ring_hit_t hits;
} ring_data_t;

// rings_all_t:
// - An array to store the all the information for all 8 rings
// - [0] = r1, [1] = r2, [2] = r3, [3] = r4
//   [4] = r5a, [5] = r5b, [6] = r5c, [7] = r6
//   bit = 0, no hit; bit = 1, hit
typedef struct
{
	ring_data_t rings[8];
} rings_all_t ;


// moller_hls:
// - main workhorse of the code where most of the logic is performed
// - Inputs:
// 		hit_dt - coincidence tolerance
// 		seed_threshold - min energy for a hit to count
// 		ring_threshold - min summed energy for a ring to be counted as hit
// 		s_fadc_hits - input stream buffer for raw FADC data
// 		rings - output stream of hit rings bitmap | [0]=1 => ring1 hit;[0]=0 => ring1 not hit 
// 		trigger - output stream of rings timing trigger bitmap
void moller_hls
(
	ap_uint<3> hit_dt, // coincidence tolerance
	ap_uint<13> seed_threshold, // minimum energy for us to look at an individual hit
	ap_uint<16> ring_threshold, // minimum summed energy (over one ring) to count a ring as hit
	hls::stream<fadc_hits_t> &s_fadc_hits, // raw FADC data input stream
	hls::stream<ring_data_t> rings // output stream for for the ring data
);

// define sub functions here

#endif
