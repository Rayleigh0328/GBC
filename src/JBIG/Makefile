CC = g++ 
CPPFLAGS = -std=c++11 -O3

OBJ := $(patsubst %.cc,%.o,$(wildcard *.cc))
OBJ += $(patsubst %.c,%.o,$(wildcard *.c))

.PHONY: all
all: $(OBJ)
	$(CC) $(CPPFLAGS) -o jbig_enc $(OBJ)

$(OBJ) : $(wildcard *.h)

.PHONY: clean
clean:
	-rm *.o
	-rm jbig_enc 
