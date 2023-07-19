# README

This directory contains a simple script that takes in a configuration file of the form:

ON/OFF		SCALAR		[SEGMENTS]

and generates an output header file. The idea is that you will use this in the FPGA design.
NOTE: Being able to load configuration files dynamically is still a WiP so this only results in 
	  a static configuration.
Ex: 

#RING 1
ON		5		[1,2,3]
#Ring 2
OFF

Segments 1,2,3 on Ring 1 are being listened to for events with a event scalar of 5 whereas Ring 2 is not being listened to. 

Note that the rings are assummed to be entered in sequential order so that the comment lines '#Ring1' are not necessary and only exist for legibility.
Likewise, it is cumbersome to type out [1,2,3,4,....,28] for the 28 segments so one can just enter [29]:

#Ring 3
ON		10		[29]

(All 28 segments on ring 3 are being listened to with a scalar of 10). 


## Running
```make CFILE=(your_config_file) HFILE=(your_output_file)```


Please see the example.conf to help illustrate how to create a .conf file
