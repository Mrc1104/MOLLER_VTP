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

const std::map<std::string, det_type> m = 
{
    {"RING_ONE", RING_ONE},
    {"RING_TWO", RING_TWO},
    {"RING_THREE", RING_THREE},
    {"RING_FOUR", RING_FOUR},
    {"RING_FIVE", RING_FIVE},
    {"RING_SIX", RING_SIX},
    {"RING_SEVEN", RING_SEVEN},
    {"TRIG_SCINT", TRIG_SCINT},
    {"PION_DET", PION_DET},
    {"SHOWER_MAX", SHOWER_MAX}
};
#endif