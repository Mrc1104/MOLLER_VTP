# Detector to Channel mapping
Stores configuration files that alter the channel to detector mapping. These are used inside moller_hls.cpp to parse what channel value corresponds to what detector and segment. Currently, configuring the det-to-chan is done via including a header file inside moller_hls.[h,cpp]

Simple script exists to generate these configuration adn header files. See Det_Chan_Config_Script/