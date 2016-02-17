CC = g++ 
CPPFLAGS = -std=c++11

OBJ := $(patsubst %.cc,%.o,$(wildcard *.cc)) ac_encoder.o prob_estimator.o elias_encoder.o

.PHONY: all
all: lib $(OBJ)
	$(CC) $(CPPFLAGS) -o main $(OBJ)

lib: 
	cd ./arithmetic_code && $(MAKE)
	cd ./elias_code && $(MAKE)

$(OBJ) : $(wildcard *.h)

.PHONY: clean
clean:
	-rm *.o
	-rm main
