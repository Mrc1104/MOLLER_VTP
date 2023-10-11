# Main Files
The three files
* main.cpp
* moller_hls.h
* moller_hls.cpp
are the main files of the repo. They meant to be used with the AMD Vitis software to program a FPGA. 

## main.cpp
Start up program. Vitis expects a testbench function so this is it. It generates the random data, intitializes the input streams, calls the top function, moller_hls(), and then parses the resulting output streams. The output streams can either be printed to the screen or to a file. 

## moller_hls.h
Header file for the functions and structures declarations used in the top function, moller_hls(). The top function houses the majority of the logic and calls subfunctions to preform specific tasks.

## moller_hls.cpp
Implementation file for the functions declared in moller_hls.h. 

### About:
The goal was to create simple ring trigger logic where we can generate fake data, parse said data,and output timing and placement of the hits for every ring across all 28 segments. We have 8 rings that span 7 sectors (which are further subdivded into 4 segments per sector for 28 sector total). We have 14 FADC (each with 16 channels) that provides energy and time data every 32ns.

### General Comments
The basis for the code came from Hanjie Liu's code for an E-calorimator cluster. I am not the most experienced programmer, especially when it comes writing firmware-type things. A lot of the code written is experimental (for me) and a lot of it was written to try different things and different approaches. I have gone through and tried to comment through the logic to help clarify things. Through trial and error with Vitis, I have found that the #pragma directives make the most difference in efficiency (especially when it comes to arrays and the "ARRAY_PARTITION" directives). 