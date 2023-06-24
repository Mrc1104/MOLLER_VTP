OPTION:=
FILE:=
configure:
	echo "Hello from makefile, target: configure"
	echo $(OPTION)
	echo $(FILE)
header:
	echo "hello from makefile, target: header"
	echo $(FILE)
