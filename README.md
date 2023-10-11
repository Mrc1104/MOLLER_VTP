# Moller_hls
I went ahead an grouped like files in named directories to help with navigation + explanation. However, this breaks #include paths and so the paths will need to be updated / fixed before the program can be run. Each subdirectory contains a README.md that (hopefully) explain what the goal of the enclosed files are. Quick word of caution, I am not the most experienced programmer, especially when it comes to writing firmware. The foundation of the code comes from Hanjie Liu's work on the ecal cluster (https://github.com/hanjie1/solid_ecal). However, the rest was done through trial and error. I am not sure how helpful this material will be, but I tried to comment out and clarify what I could. 

# Layout
There are two git repos,
1. MOLLER_VTP (this one)

and 

2. VXS-Crate-2 (https://github.com/Mrc1104/VXS-Crate-2)

The two are very similar projects but with different scopes. MOLLER_VTP is for only the thin quartz detectors wheras the VXS-Crate-2 is for the shower max, pion detectors, and trigger scintillator pairs. Both repos, however, were trying to set up basic trigger functionality: can we see what detector was hit, which segment it occured in, and the timing of the hit. 
Moller_VTP was written first and served as the primary testing ground for ideas and changes that were ported over to VXS-Crate-2. Therefore, MOLLER_VTP has a higher likelihood of being disoriented so I spent most of the time explaining this repository. 

Since I grouped files in common directories, I reccomend checking out the other branches where I will have a compileable branch. That being said, lets start with the primary group:

## Main Files
These are the primary files of the repository. They include 
* main.cpp

The testbench file needed to generate fake fadc data, intilize variables to be passed to the top function, and output the bitmasks the top function returns. Fanins and Fanouts occur using the hls::stream<> data type as input parameters into the top function. 
* moller_hls.cpp

The top function where the fake fadc data is parsed and distributed to the appropriate detectors. This is where the total # of hits per ring is calculated and the bitmasks for each detector and segment is created.
* moller_hls.h

The header file that holds the function and struct declarations for moller_hls.cpp. 

## Supporting_Headers
These are header files that define global variables, std::maps, struct, and etc. that are used by several different files.
* variables.h

Defines N_SLOT (# of fadc slots) and N_CHAN (# of fadc channels, each fadc has 16 channels). These are the two most important global variables. Needed for helper files as well as the main moller_hls files. 
* detector_type.h

This declares the detector enum that is used to map channel to detector in a more legible manner. Used in serveral helper files and the main moller_hls files. 
* chan_map.h

Holds a detector to channel map struct that is required for the det_chan maps that are stored as arrays.  
* std_map.h 

A std::map that converts strings to the detector enum or vice versa. Used in the det-chan configuation scripts (see Det_Chan_Config_script/)
* data_stream.h

Three arrays that store permenant fake data. Used for multi-run testing where we know the desired output.

## Supplements
Currently only holds additional information regarding the splitting of events into current_hit and pre_hit bins. Essentially, we shift the time window we are interested so that half the data we fanout comes from the previous 32ns fadc clock cycle and half comes from the current 32ns fadc clock cycle. 

# Configuration and Configuration Scripts
One thing MOLLER_VTP and VXS-Crate-2 sought to do was have the ability to choose what fadc channels correspond to what detector and detector segments. Additionally, we also wanted the ability to set scalars and to select which detectors or segments we would listed to. This culminates into having configuration files. Eventually, we would get to loading these configuration files dynamically into the FPGA, but we are not there yet. So, for now, we are simply "loading configuration files" statically via header files. There are two types of configuration files:
1. Trigger Configuration
Which detector and segments do we care about and with what scalars.
2. Detector to Channel Configuration
Which fadc channel corresponds to which detector and segment. 

## Trigger Configuration
Trigger Configuration header file holds three pieces of information:
* ap_uint<8> ring_trigger_config_bitmap

A 8-bit bitmask that indicates which rings we are listening to.
* ap_uint<28> segment_trigger_config_bitmap[8]

An array of 8, 28bit bitmasks that indicate which segments we care about for each ring
* ap_uint<8> ring_trigger_scalars[8]

An array of 8, 8bit ring countdown scalars. The ring will not detect a hit until the scalar reaches zero.  

There exists a helper file called trigger_conf.cpp. The C++ file will take a trigger configuration file and parse it into a useable header file that can be included in moller_hls.[h,cpp], 
(See trigger_config/ for examples and more information)

## Detector to Channel Configuration
The det_chan is an array of chan_map structs. This array is used in moller_hls.cpp to parse which channel corresponds to which detector and segment. It additionally stores secondary detector information like which sub-ring Ring5 is (5a,5b,5c) or which channel the partner scintillator in the scintillator pair resides in (to check for coincidences)
There exists three helper functions to help generate these det-chan-mapping arrays. A shell script that takes in information and passes it to a makefile. The makefile then compiles and calls either configure.cpp to create a detector to channel configuration file or the parser.cpp which takes the configuration file and creates an array. 
(See Det_Chan_Config_Script/ for more information and examples) 

# Misc. Files
There exists several output files that were used to verify the output of the moller_hls logic. I primarily worked through a preset of fake data and used that to compare (via the output) if the logic was working properly or not. They are, for the most part, inconsequential. 

# Summary
That covers the main grouped files. To recap, the primary files are `main.cpp`, `moller_hls.cpp`, and `moller_hls.h`. Important header files that are used by a variety of files are located in the Supporting_Headers folder. Basic configuration abilties exist but are done in a very obtuse and static way via arrays stored in header files. To keep the structure consistent, helper files exist to create these arrays. Each main subdir has a README.md with more information if interested.

# Contact
If you have any questions, please feel free to contact me either via my OU email or jeffersonlab email.

Ryan Conaway

mc321015@ohio.edu

mrc@jlab.org