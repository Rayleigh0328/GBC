CC = g++
OBJ = ac_test.o ac_decoder.o ac_encoder.o prob_estimator.o elias_encoder.o elias_decoder.o 
VPATH = ../elias_code/

all: gen_input test

gen_input: ac_input_gen.cpp
	$(CC) ac_input_gen.cpp -o input_gen

test: $(OBJ)
	$(CC) -o ac_test $(OBJ)

ac_test.o: ac_test.cpp

ac_decoder.o: ac_decoder.h ac_def.h prob_estimator.h elias_encoder.h

ac_encoder.o: ac_encoder.h ac_def.h prob_estimator.h elias_decoder.h

prob_estimator.o: prob_estimator.h

elias_encoder.o: elias_encoder.h

elias_decoder.o: elias_decoder.h

.PHONY: run
run:
	./input_gen
	./ac_test
	diff -s ac_input ac_output 

.PHONY: clean
clean:
	-rm *.o
	-rm ac_input ac_output ac_test input_gen label_file



