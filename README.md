# MOLLER_VTP
Fork of hanjie1/solid_ecal. Trigger design in vivado_hls 

# About:
This is a simple ring trigger system. Using Vitis HLS (v2023.1), we can implement C/C++ code as HDL that can be used to programm FPGAs. 

Background: We have a detector that has 8 rings that span 7 sectors (which are further subdivded into 4 segments per sector for 28 sector total). We have 14 FADC
(each with 16 channels) that provides energy and time data every 32ns.

Goal: We want to see the timing and placement of the hits for every ring across all 28 segments.

Important files:

- main.cpp: the test bench file that tests the logic
- moller_hls.h: header file that defines structs and functions
- moller_hls.cpp: Implementation file of moller_hls.h




## Contact:

Name: Ryan Conaway

Email: mrc@jlab.org

University: Ohio University

Advisor: Dr. Paul King
