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
// - vxs_ch[ 208 ] to vxs_ch[ 224 ]: VME slot 19, ch 0 to 15 FADC channels // TODO: this should only go to 224 TOTAL channels
//
#define N_CHAN_SEC 5; // number of FADC channels per sector // TODO: not sure what this val should be at the moment
typedef struct
{
	hit_t vxs_chan[N_CHAN_SEC];
} fadc_hits_t;

// ring_t:
// - e: energy measured in a ring (summed over all 28 segements)
// - t: ring time, timestamped from ring central hit time appended to coarse 32ns frame counter
// - nhits: number of hit channels in ring
typedef struct
{
	ap_uint<16> e;
	ap_uint<3> t;
	ap_uint<5> nhits;
} ring_t;

typedef struct
{
	ring_t aring[8]; // we have eight rings
} rings_all_t;

// bitreg_t:
// - bit register 
// - Two uses: a timing bit register and a ring bit register
// - Timing register: [0]=>0ns, [2]=>8ns, ..., [7]=>28ns
// - Ring resiter: bits [0] - [7] correspond to the 8 rings. If [0] = 1 => ring one as a hit
// - If [0] = 0 => ring one was not hit. etc.
// - Note: I need one for each ring for the timing and the ring registers so its an array of these// - 8-bit registers
typedef struct
{
	ap_uint<8> bitreg[8];
} bitreg_t;

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
	hls::stream<bitreg_t> rings, // stream for the bitmap of which rings were hit
	hls::stream<bitreg_t> trigger // stream for the timing triggers
);

// define sub functions here

#endif
