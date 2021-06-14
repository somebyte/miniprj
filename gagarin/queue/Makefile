CPP      = g++
CPPFLAGS = -std=c++11

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $<

all: queuetest queueif

queuetest: queue.o queuetest.o
	$(CPP) $(CPPFLAGS) -o $@ $^

queueif: queue.o queueif.o
	$(CPP) $(CPPFLAGS) -o $@ $^

test: all
	./queuetest.py
	./queueiftest.py

clean:
	rm -f *.o
	rm -rf __pycache__
	rm -f queuetesting.py
	rm -f queueres.py

distclean: clean
	rm -f queuetest
	rm -f queueif

.PHONY: all test clean distclean
