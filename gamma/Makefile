%.o: %.c
	${CC} -fPIC -c $< 

all: libgamma example

libgamma: lib.o
	${CC} -shared -o $@.so $^

example: example.o 
	${CC} -o $@ $^ -L. -lgamma

clean:
	rm -f *.o

distclean: clean
	rm -f libgamma.so example

.PHONY: all clean distclean
