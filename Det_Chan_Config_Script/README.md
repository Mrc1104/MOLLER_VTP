# Files:
Four files 
* map.csh
* configuration.cpp
* parser.cpp
* script.mk
are helper files to generate channel-to-detector maps. This basically was a way for me to practice shell-scripting and C++ while providing a systematic way to make different channel to detector maps needed for moller_hls testing (see main_files/). 

I do not expect this to be of any use so I'll be brief with my description.

## General Flow: 
1. `map.csh`
Shells script that prompts the user for information. Passes this information to the script.mk makefile. 
2. `configuration.cpp`
Asks the user for information regarding what channels correspond to what detector + segments + secondary detector information (ie the chan # for other scintillator in scintillator pairs). Generate a configuration file that can then be passed to the parser.cpp file (see chan_map/det_map_naive.conf for an exampe of these config files).
3. `parser.cpp`
Takes the input configuration file and converts it to a header file. The configuration is stored as an array (see chan_map/array1.h). 

The idea was that we will eventually dynamically load configurations to the FPGA at runtime. The mechanism for this dynamic configuration is currently unknown to me. One would need to ask Ben for more information regarding this. 

# General Comments
Again, this served mainly as a practice tutorial for me and I doubt it will have any value. It was a fun little side project. Error checking is minimal or N/A. 