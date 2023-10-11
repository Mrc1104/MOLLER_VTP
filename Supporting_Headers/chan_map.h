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


#endif
