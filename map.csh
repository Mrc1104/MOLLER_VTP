#! /bin/tcsh
clear
set fileName = "`find *.csh`"
set num_args = $#
set paraPath = ""
set headerPath = ""
set i = 1;
while ($i <= $num_args)
	if( $argv[$i]:q == "-p" ) then
		@ j = ($i + 1)
		set paraPath = $argv[$j]
	endif
	if( $argv[$i]:q == "-s" ) then
		@ j = ($i + 1)
		set headerPath = $argv[$j]
	endif
	if( $argv[$i]:q == "-h" ) then
		echo "########################################"
		echo "Help: $fileName -[PARAMETER]"
		echo "-p:"
		echo "Path to Parameter File"
		echo "If the file already exists, overwrites it or modifies it depending whether you choose 1) Create or 2) Modify"
		echo ""
		echo "-s:"
		echo "Path to Array Header File"
		echo "If the file already exists, it is overwritten"
		echo ""
		echo "-h:"
		echo "Help"
		echo "Displays this Screen"
		echo "########################################\n"
		exit 
	endif

	
	
	@ i = $i + 1
end

echo "Welcome\n"
echo "Channel-Detector Map Generator:"
echo "You can generate map parameter files or construct map arrays to be used in Vitis HLS"
echo "The Number of Slots (N_SLOT) and Number of Channels (N_CHAN) are defined in variables.h"
echo "What would you like to do? (run with -h for help)\n"

set RUN = 1
while ( $RUN == 1 )
	echo "\n########################################\n"
	echo "1) Create New Parameter File"
	echo "2) Modify Existing Parameter File"
	echo "3) Create an array header file"
	echo "4) To Quit"
	echo "\n########################################\n"
	echo -n "Input: "
	
	set userInput = $<
	while ( $userInput < 1 || $userInput > 4 )
		echo "Invalid Input"
		echo -n "Input: "
		set userInput = $<
	end

	if ( $userInput == 1 ) then
		if( $paraPath == "") then
			echo -n "No Parameter File given; please do so now: "
			set paraPath = $<
		endif
		if( -e $paraPath ) then
			echo "Warning! The file $paraPath alrady exists and will be overwritten"
			echo "Do you wish to continue? (n:0/y:1):"
			set cont = $<
			if( $cont == 1 ) then
				echo "Continuing..."
			else 
				echo "Invalid input. Aborting..."
				exit	
			endif
		endif
		make -f script.mk configure OPTION=1 PFILE=$paraPath
	endif
	if ( $userInput == 2) then
		if( $paraPath == "") then
			echo -n "No Parameter File given; please do so now: "
			set paraPath = $<
		endif
		if( -e $paraPath ) then
			echo "Warning! The file $paraPath alrady exists and will be overwritten"
		else
			echo -n "No valid path given (provided: $paraPath); please do so now: "
			set paraPath = $<
		endif
		make -f script.mk configure OPTION=2 PFILE=$paraPath
	endif
	
	if ( $userInput == 3 ) then
		if( $paraPath == "") then
			echo -n "No Parameter File given; please do so now: "
			set paraPath = $<
		endif
		if( $headerPath == "") then
			echo -n "No Header File given; please do so now: "
			set headerPath = $<
		endif
		if( -e $headerPath) then
			echo "Warning! The file $headerPath alrady exists and will be overwritten"
			echo "Do you wish to continue? (n:0/y:1):"
			set cont = $<
			if( $cont == 1 ) then
				echo "Continuing..."
			else 
				echo "Invalid input. Aborting..."
				exit	
			endif
		endif
		make -f script.mk header PFILE=$paraPath HFILE=$headerPath
	endif

	if ( $userInput == 4 ) then
		set RUN = 0
	endif

	echo "Anything else?"
end
clear
