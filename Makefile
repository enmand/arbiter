all: .cpp.so main

main:
	g++ -I/opt/local/include -L/opt/local/lib daemon.so -lmongoclient -lboost_thread-mt -lboost_filesystem -lboost_program_options -lboost_system arbiter.cpp -o arbiter

.cpp.so:
	g++ -Wall -c -o daemon.so daemon.cpp

clean:
	rm -f *.so
	rm -f arbiter
	rm -f pid.arbiter