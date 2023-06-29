#ifndef CHAN_MAP_H
#define CHAN_MAP_H

#include <ap_int.h>
#include <map>
#include "detector_type.h"


typedef struct 
{
    det_type DET_ID;
    ap_uint<5> SEG_NUM;
    ap_int<9> SUB_ELEMENT;
} chan_map;

// const std::map<std::string, det_type> m = 
// {
//     {"RING_ONE", RING_ONE},
//     {"RING_TWO", RING_TWO},
//     {"RING_THREE", RING_THREE},
//     {"RING_FOUR", RING_FOUR},
//     {"RING_FIVE", RING_FIVE},
//     {"RING_SIX", RING_SIX},
//     {"RING_SEVEN", RING_SEVEN},
//     {"TRIG_SCINT", TRIG_SCINT},
//     {"PION_DET", PION_DET},
//     {"SHOWER_MAX", SHOWER_MAX}
// };
// const std::map<det_type, std::string> m2 = 
// {
//     {RING_ONE, "RING_ONE"},
//     {RING_TWO, "RING_TWO"},
//     {RING_THREE, "RING_THREE"},
//     {RING_FOUR, "RING_FOUR"},
//     {RING_FIVE, "RING_FIVE"},
//     {RING_SIX, "RING_SIX"},
//     {RING_SEVEN, "RING_SEVEN"},
//     {TRIG_SCINT, "TRIG_SCINT"},
//     {PION_DET, "PION_DET"},
//     {SHOWER_MAX, "SHOWER_MAX"}
// };

typedef struct
{
    ap_uint<4> DET_ID;
    ap_uint<5> SEG_NUM;
    ap_int<9> SUB_ELEMENT;
} chan_map_integer;

const std::map<std::string, int> m =
{
    {"RING_ONE", 1},
    {"RING_TWO", 2},
    {"RING_THREE", 3},
    {"RING_FOUR", 4},
    {"RING_FIVE", 5},
    {"RING_SIX", 6},
    {"RING_SEVEN", 7},
    {"TRIG_SCINT", 8},
    {"PION_DET", 9},
    {"SHOWER_MAX", 10}
};
const std::map<ap_uint<4>, std::string> m2 =
{
    {0, "RING_ONE"},
    {1, "RING_TWO"},
    {2, "RING_THREE"},
    {3, "RING_FOUR"},
    {4, "RING_FIVE"},
    {5, "RING_SIX"},
    {6, "RING_SEVEN"},
    {7, "TRIG_SCINT"},
    {8, "PION_DET"},
    {9, "SHOWER_MAX"}
};
const std::map<det_type, ap_uint<4>> m3 =
{
    {RING_ONE, 0},
    {RING_TWO, 1},
    {RING_THREE, 2},
    {RING_FOUR, 3},
    {RING_FIVE, 4},
    {RING_SIX, 5},
    {RING_SEVEN, 6},
    {TRIG_SCINT, 7},
    {PION_DET, 8},
    {SHOWER_MAX, 9}
};
#endif