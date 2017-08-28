CFLAGS = -I./include -Wall -ansi -pedantic
CC = gcc

all: bin/morpion bin/server bin/client tests/benchmark

tests/benchmark: obj/grille.o obj/morpion.o obj/joueur.o obj/strategies.o obj/benchmark.o obj/text_interface.o
	$(CC) $(CFLAGS) obj/joueur.o obj/morpion.o obj/grille.o obj/strategies.o obj/benchmark.o obj/text_interface.o -o tests/benchmark

bin/morpion: obj/grille.o obj/morpion.o obj/joueur.o obj/strategies.o obj/main.o obj/text_interface.o
	$(CC) $(CFLAGS) obj/joueur.o obj/morpion.o obj/grille.o obj/strategies.o obj/main.o obj/text_interface.o -o bin/morpion

bin/server: obj/grille.o obj/morpion.o obj/joueur.o obj/strategies.o obj/server.o obj/text_interface.o
	$(CC) $(CFLAGS) obj/joueur.o obj/morpion.o obj/grille.o obj/strategies.o obj/server.o obj/text_interface.o -lzmq -o bin/server

bin/client: obj/grille.o obj/morpion.o obj/joueur.o obj/client.o obj/text_interface.o
	$(CC) $(CFLAGS) obj/joueur.o obj/morpion.o obj/grille.o  obj/strategies.o obj/client.o obj/text_interface.o -lzmq -o bin/client

obj/benchmark.o: src/benchmark.c
	$(CC) $(CFLAGS) -c src/benchmark.c -o obj/benchmark.o

obj/grille.o: src/grille.c include/grille.h
	$(CC) $(CFLAGS) -c src/grille.c -o obj/grille.o

obj/morpion.o: src/morpion.c include/morpion.h
	$(CC) $(CFLAGS) -c src/morpion.c -o obj/morpion.o

obj/strategies.o: src/strategies.c include/strategies.h
	$(CC) $(CFLAGS) -c src/strategies.c -o obj/strategies.o

obj/joueur.o: src/joueur.c include/joueur.h
	$(CC) $(CFLAGS) -c src/joueur.c -o obj/joueur.o

obj/text_interface.o: src/text_interface.c include/user_interface.h
	$(CC) $(CFLAGS) -c src/text_interface.c -o obj/text_interface.o

obj/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

obj/server.o: src/server.c
	$(CC) $(CFLAGS) -c -std=gnu99 src/server.c -o obj/server.o

obj/client.o: src/client.c
	$(CC) $(CFLAGS) -c -std=gnu99 src/client.c -o obj/client.o

test:
	echo a faire

doc:
	doxygen

clean:
	rm -rf obj/*.o bin/*

zip:
	mkdir "${USER}"
	cp Makefile *.c *.h "${USER}"
	tar czf "${USER}".tgz "${USER}"
	rm -rf "${USER}"

