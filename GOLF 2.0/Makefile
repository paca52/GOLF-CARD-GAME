CC=g++
FLAGS=-O1 -Wall -Wno-missing-braces -Iinclude/ -Llib/ -lraylib -lopengl32 -lgdi32 -lwinmm -static-libstdc++ -static-libgcc -static

all: game
	game

game: main.o cards.o functions.o hashtables.o
	$(CC) $^ -o $@ $(FLAGS)

main.o: main.cpp
	$(CC) -c $^

functions.o: functions.cpp include/functions.h
	$(CC) -c functions.cpp

cards.o: cards.cpp include/cards.h
	$(CC) -c cards.cpp

hashtables.o: hashtables.cpp include/hashtables.h
	$(CC) -c hashtables.cpp

clean:
	del game .\*.o
