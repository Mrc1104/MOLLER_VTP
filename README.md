# MOLLER_VTP
Fork of hanjie1/solid_ecal. Trigger design in vivado_hls 

# Layout
There are two git repos,
1. MOLLER_VTP (this one)

and 

2. VXS-Crate-2 (https://github.com/Mrc1104/VXS-Crate-2)

The two are very similar projects but with different scopes. MOLLER_VTP is for only the thin quartz detectors wheras the VXS-Crate-2 is for the shower max, pion detectors, and trigger scintillator pairs. Both repos, however, were trying to set up basic trigger functionality: can we see what detector was hit, which segment it occured in, and the timing of the hit. 

Moller_VTP was written first and served as the primary testing ground for ideas and changes that were ported over to VXS-Crate-2. Therefore, MOLLER_VTP has a higher likelihood of being disoriented so I spent most of the time explaining this repository. 

# Main Files
The three files
* main.cpp          -- testbench file
* moller_hls.cpp    -- top function file
* moller_hls.h      -- header file

are the main files of the repo. They meant to be used with the AMD Vitis software to program a FPGA. 

## main.cpp
Start up program. Vitis expects a testbench function so this is it. It generates the random data, intitializes the input streams, calls the top function, moller_hls(), and then parses the resulting output streams. The output streams can either be printed to the screen or to a file. 

## moller_hls.h
Header file for the functions and structures declarations used in the top function, moller_hls(). The top function houses the majority of the logic and calls subfunctions to preform specific tasks.

## moller_hls.cpp
Implementation file for the functions declared in moller_hls.h. 

### About:
The goal was to create simple ring trigger logic where we can generate fake data, parse said data,and output timing and placement of the hits for every ring across all 28 segments. We have 8 rings that span 7 sectors (which are further subdivded into 4 segments per sector for 28 sector total). We have 14 FADC (each with 16 channels) that provides energy and time data every 32ns. We also want to have a channel-to-detector map so we can have abritrary channel-detector configurations. Likewise, we want to be able to select which detectors, which segments, and with what scalars that we want. These two configurations are done very statically using configuration scripts
* map.csh - Takes input from the user, calls the script.mk, and generates chan-det mapping arrays. 
* trigger_config/trigger_conf.cpp - takes a trigger configuration file and generates a header file. 

### General Comments
The basis for the code came from Hanjie Liu's code for an E-calorimator cluster. I am not the most experienced programmer, especially when it comes writing firmware-type things. A lot of the code written is experimental (for me) and a lot of it was written to try different things and different approaches. I have gone through and tried to comment through the logic to help clarify things. Through trial and error with Vitis, I have found that the #pragma directives make the most difference in efficiency (especially when it comes to arrays and the "ARRAY_PARTITION" directives). 

### Configration Files
The idea was to eventually be able to load configuration file to the FPGA dynamically. As if now, all configuration files are included in moller_hls.[h,cpp] as header files. There are trigger configuration files (what rings/segments are enabled and with what prescalars) and detector-to-channel-map configuration files (what fadc channel corresponds to which detector and segment). See trigger_config/ and Det_Chan_Config_Script/ for more information and examples. 

Both folders also contain helper scripts that will generate these configuration headers.

## Contact:

Name: Ryan Conaway

Email: mrc@jlab.org
