PRJ      = progression
CPP      = g++
CPPFLAGS = -std=c++11

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $<

all: $(PRJ)

$(PRJ): $(PRJ).o
	$(CPP) $(CPPFLAGS) -o $@ $^ -lpthread

clean:
	rm -f *.o

distclean: clean
	rm -f progression

.PHONY: all clean distclean

