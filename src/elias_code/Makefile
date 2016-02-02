CC = g++
OBJ = elias_encoder.o elias_decoder.o elias_tester.o

all: $(OBJ)
	$(CC) -o elias_test $(OBJ)

elias_encoder.o: elias_encoder.h

elias_decoder.o: elias_decoder.h

elias_tester.o: elias_encoder.h elias_decoder.h

.PHONY: run
run:
	./elias_test
	diff -s elias_input elias_output

.PHONY: clean 
clean:
	rm *.o
	rm elias_test elias_input elias_output

