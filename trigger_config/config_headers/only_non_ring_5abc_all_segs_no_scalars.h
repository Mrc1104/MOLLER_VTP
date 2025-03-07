#pragma once
#include <ap_int.h>

const ap_uint<8> ring_trigger_config_bitmap = 143;
// [1][0][0][0][1][1][1][1]
// MSB ----------------- LSB

const ap_uint<28> segment_trigger_config_bitmap[8] = 
{
// [1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1]
268435455, // Ring 1
// [1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1]
268435455,
// [1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1]
268435455,
// [1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1]
268435455,
// [0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0]
0,
// [0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0]
0,
// [0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][0]
0,
// [1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1]
268435455 // Ring 8
};
const ap_uint<8> ring_trigger_scalars[8] =
{
 0, // Ring 1
 0,
 0,
 0,
 0,
 0,
 0,
 0  // Ring 8
};