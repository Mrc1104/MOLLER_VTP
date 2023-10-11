# Supporting Headers
There are five supporting header file
* variables.h
* detector_type.h
* chan_map.h
* std_map.h
* data_stream.h

## variables.h
This holds the main global variable defintions that are used in several different files:
1. N_SLOT -- # of FADC slots
2. N_CHAN -- # of FADC channels

## detector_type.h
The detector enum used used in a variety of files

## chan_map
Basic struct that is used for the det_chan mappings to hold detector ID, segment number, and sub-element. 

## std_map.h
A map that converts a string to the detector_type enum or vice versa (map m and map m2 respectivly). Mainly used in generating det_chan mappings. 

## data_stream.h
Holds three arrays of pre-set "random" data used for testing. Vitis has some odd behavior with generating consistent data so I prepared three arrays and saved them so they could be used over and over again with their desired output being known. 