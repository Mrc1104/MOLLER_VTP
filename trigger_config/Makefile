CFILE:=
HFILE:=

a.out: trigger_conf.cpp 
	g++ -o trigger_setup -I ../include/ -Wfatal-errors trigger_conf.cpp
	trigger_setup $(CFILE) $(HFILE) 
