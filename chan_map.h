#include <ap_int.h>
#include "detector_type.h"
typedef struct 
{
    det_type DET_ID;
    ap_uint<5> SEG_NUM;
    ap_uint<8> SUB_ELEMENT;
} chan_map;
