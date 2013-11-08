CC=g++
CFLAGS=-std=c++11
LIBOBJS=computer_player.o field.o game.o human_player.o

.PHONY: all clean test

all: test tictactoe

tictactoe: $(LIBOBJS) main.o
	$(CC) $(CFLAGS) -o $@ $^

testtictactoe: $(LIBOBJS) test_main.o
	$(CC) $(CFLAGS) -o $@ $^

test: testtictactoe
	testtictactoe

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm tictactoe *.o
