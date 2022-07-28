CC=g++
CFLAGS=-I -g -std=c++17
DEPS = DeltaCalculator.h FileIO.h ArgumentHandler.h
OBJ = main.o FileIO.o DeltaCalculator.o ArgumentHandler.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rollhashdiff: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o rollhashdiff