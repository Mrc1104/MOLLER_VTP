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
		echo "Anything else?"
	endif

	if ( $userInput == 2) then
		echo "Anything else?"
	endif

	if ( $userInput == 3 ) then
		echo "Anything else?"
	endif

	if ( $userInput == 4 ) then
		set RUN = 0
	endif
end
clear
