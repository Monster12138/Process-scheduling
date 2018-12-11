all:process_scheduling
process_scheduling:task.h sys.h process_scheduling.cpp
	g++ -g $^ -o $@
