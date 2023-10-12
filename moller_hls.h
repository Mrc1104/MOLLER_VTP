#ifndef MOLLER_HLS_H
#define MOLLER_HLS_H

#include <ap_int.h>
#include <hls_stream.h>

#include "chan_map.h" // channel to detector mapping 
#include "detector_type.h" // detector enum
#include "variables.h" // N_CHAN and N_SLOT definitions

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
// ...
// - vxs_ch[ 112 ] to vxs_ch[ 127 ]: VME slot 10, ch 0 to 15 FADC channels
// - (VXS switch A and B are at VME slot positions 11,12 so the FADC cannot be installed here
// - vxs_ch[ 144 ] to vxs_ch[ 159 ]: VME slot 5, ch 0 to 15 FADC channels
// ...
// - vxs_ch[ 208 ] to vxs_ch[ 223 ]: VME slot 19, ch 0 to 15 FADC channels
// Comment:
// - N_CHAN is defined in variables.h
//
typedef struct
{
	hit_t vxs_chan[N_CHAN];
} fadc_hits_t;


// ring_hit_t:
// - Hit data for a single ring
// - e: 		sum of the energy of all the hits on a ind. ring
// - t: 		ring time measured at the central hit time
// - nhits:		number of hits on an ind. ring
// - segment: 	bit map for the 28 segments in our detector
// - bitbuffer: bit buffer to make the struct multiple of 8
typedef struct
{
	ap_uint<16> e;
	ap_uint<6> nhits;
	ap_uint<28> segment;
	ap_uint<14> bpadding;

} ring_hit_t;

typedef struct
{
	ring_hit_t r[8];
} ring_all_t;


// trigger_t:
// - trig: 			   bitmap for time - [0]=>0ns, [1]=>4ns, [2]=>8ns, ..., [7]=28ns, when bit=0 no trigger, when bit=1 trigger
// - 8-element array, one for each ring: [0] = r1, [1] = r2, ..., [7] = r7; when bit=0 no time_trigger, when bit=1 time_trigger
typedef struct
{
	ap_uint<8> trig[8];
} trigger_t;

// ring_trigger_t:
// - ring trig: bitmap for ring hit - [0]=r0, [1]=r1, [2]=r2, ..., [7]=r6; when bit=0 no ring_trigger, when bit=1 ring_trigger
typedef struct 
{
	ap_uint<8> ring;
} ring_trigger_t;

// raw_counter_t
// - counter: a raw counter that increases whenever a ring is hit (ignores any scalars and configurations)
typedef struct
{
	ap_uint<16> counter;
} raw_counter_t;

// ring_all_counter_t 
// - ring_counter[8]: an arr for all 8 rings that stores the raw counter information
typedef struct
{
	raw_counter_t ring_counter[8];
} ring_all_counter_t;

/* Functin declarations */

// moller_hl:
// - main workhorse of the code where most of the logic is performed
// - Inputs:
// 		hit_dt						 - coincidence tolerance
// 		seed_threshold 				 - min energy for a hit to count
// 		ring_threshold 				 - min summed energy for a ring to be counted as hit
// 		s_fadc_hits 				 - input stream for raw FADC data
// 		rings 						 - output stream of hit rings bitmap | [0]=1 => ring1 hit;[0]=0 => ring1 not hit 
// 		trigger 					 - output stream of rings timing trigger bitmap
// 		s_ring_all   				 - output stream for the bitmap of ring hits
// 		s_ring_all_counter 		     - output of a raw ring counter for all rings
/*
 * moller_hls():
 * Brief: 		
 * 				Main workhorse where most of the logic occurs. Takes in data streams,
 * 				parses the data, generates ring masks, segment masks, and timing masks. 
 * 				Returns via data streams
 * Description: Takes in information from the fadc, parses that data, and generates required
 * 				bitmasks by calling subfunctions (see sub function definitions below)
 * 				The data, when parsed, gets allocated to the appropriate ring buffer 
 * 				and ring masks, timing masks, and segment masks for each ring are created
 * 				and returned via the output streams 
 * 				
 * Para:
 * 		        hit_dt						 - coincidence tolerance
 * 				seed_threshold 				 - min energy for a hit to count
 * 				ring_threshold 				 - min summed energy for a ring to be counted as hit
 * 				s_fadc_hits 				 - input stream for raw FADC data
 * 				rings 						 - output stream of hit rings bitmap | [0]=1 => ring1 hit;[0]=0 => ring1 not hit 
 * 				trigger 					 - output stream of rings timing trigger bitmap
 * 				s_ring_all   				 - output stream for the bitmap of ring hits
 * 				s_ring_all_counter 		     - output of a raw ring counter for all rings
*/
void moller_hls
(							
	ap_uint<13> energy_threshold, 			   
	ap_uint<16> ring_threshold, 				
	hls::stream<fadc_hits_t> &s_fadc_hits, 
	hls::stream<trigger_t> &s_trigger,
	hls::stream<ring_trigger_t> &s_ring_trigger,
	hls::stream<ring_all_t> &s_ring_all,
	hls::stream<ring_all_counter_t> &s_ring_all_counter
);

/* define sub functions here */


/*
 * add_ring_data():
 * Brief: 		
 * 				Parses FADC channel data and sums it to the appropriate ring. Creates the rings
 * 				segment bitmask. Each fadc channel corresponds to a different ring and segment
 * Para:
 * 				int ringNum       - ring number to add hit_data to 
 * 				int hit_segment   - segment number that was hit
 *				hit_t hit_data    - event data from the fadc
 *				ring_hit_t* rings - struct that stores the ring data and segment bitmask
*/
void add_ring_data(
	int ringNum,
	int hit_segment,
	hit_t hit_data,
	ring_hit_t* rings
);

/*
 * make_ring_bitmap():
 * Brief: 		Creates the ring bitmask if a pre-set ring-energy threshold has been set. 
 * 				Stored in *rings 
 * Description: Creates a 8 bit bitmask that corresponds to which rings were hit
 * 				Bitmask for ring hit - [0]=r0, [1]=r1, [2]=r2, ..., [7]=r6; 
 * 				when bit=0 no ring_trigger, when bit=1 ring_trigger
 * Para:
 * 				ring_hit_t *rings   	   - ring hit bitmask to be updated
 * 				ap_uint<16> ring_threshold - global ring threshold that needs to be satisfied
 * 											  before a hit is considered to be true
 * Comment:
 * 				A global ring_threshold might be unnecessary. A per ring threshold would make
 * 				more sense. Possible refactoring required.
*/
ring_trigger_t make_ring_bitmap(ring_hit_t* rings, ap_uint<16> ring_threshold);

/*
 * make_timing_bitmap():
 * Brief: 		Creates timing bit mask that is stored in *ptrigger
 * Description: We are shifting our timing window down to look at 4 time ticks current events
 * 				and 4 ticks of the previous events. Take in the current time trigger data and creates
 * 				the appropriate time trigger mask.
 * Para:
 * 				int ringNum    		- ring number that the data corresponds to
 * 				hit_t hit_data 		- event information
 * 				tirgger_t *ptrigger - The trigger bit mask for the current ring that is 
 * 									  to be updated and returned  
*/
void make_timing_bitmap(
	int ringNum,
	hit_t hit_data,
	trigger_t *ptrigger
);
/*
 * make_event():
 * Brief: 		
 * 				Takes in pre_hit and curr_hit, determines which one falls within the 
 * 		  		proper window, the returns that event
 * Description: 
 * 				We are shifting our timing window down to look at 4 time ticks current events
 * 				and 4 ticks of the previous events (the efficacy of this is debatable). 
 * 				We check prev_hit and curr_hit time stamps to determine if they fall within the right
 * 				window then return that event. 
 * Para:
 * 				hit_t pre_hit - previous event data that had the appropriate timining data
 * 				hit_t cur_hit - current event data that had the appropriate timining data
 * Returns:
 * 				the event that falls within the desired timing window
 * 				
 * Comment:		
 * 				The channel number is 1 to 1 with the input channel data since cur_hit and pre_hit use the 
 * 				same channel-to-detector map
*/
hit_t make_event(
	hit_t pre_hit, 
	hit_t cur_hit
);
#endif
