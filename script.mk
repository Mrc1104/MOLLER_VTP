OPTION:=
PFILE:=
HFILE:=
configure: configure.o
	echo "Hello from makefile, target: configure"
	echo $(OPTION)
	echo $(PFILE)
	./configure $(OPTION) $(PFILE)
header: parser.o
	echo "hello from makefile, target: header"
	echo $(PILE)
	echo $(HILE)
	./parser $(PFILE) $(HFILE)
configure.o: configure.cpp
	g++ -o configure configure.cpp
parser.o: parser.cpp
	g++ -o parser parser.cpp
