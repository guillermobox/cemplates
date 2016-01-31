CFLAGS += -g

all: main

.PHONY: clean all

clean:
	rm -f *.o cemplates

main: main.o cemplates.o
