OPTION:=
PFILE:=
HFILE:=
configure: configure.o
	./configure $(OPTION) $(PFILE)
header: parser.o
	./parser $(PFILE) $(HFILE)
configure.o: configure.cpp variables.h
	g++ -o configure configure.cpp
parser.o: parser.cpp variables.h
	g++ -I include/ parser.cpp -o parser
