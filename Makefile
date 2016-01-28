CFLAGS += -g

all: cemplates

.PHONY: clean all

clean:
	rm -f *.o cemplates

cemplates: cemplates.o
