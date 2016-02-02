CC = g++
OBJ = huffman_codec.o huffman_tester.o

all:$(OBJ)
	$(CC) -o test $(OBJ)

huffman_codec.o: huffman_codec.h

huffman_tester.o: huffman_codec.h

.PHONY: test
test:
	./test

.PHONY: clean
clean:
	-rm *.o test



