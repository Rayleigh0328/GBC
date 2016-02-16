CC = g++ 
CPPFLAGS = -std=c++11

OBJ := $(patsubst %.cc,%.o,$(wildcard *.cc))

.PHONY: all
all: $(OBJ)
	$(CC) $(CPPFLAGS) -o jbig_enc $(OBJ)

$(OBJ) : $(wildcard *.h)

.PHONY: clean
clean:
	-rm *.o
	-rm main
