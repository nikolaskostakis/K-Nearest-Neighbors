all:
	gcc -c main.c;
	gcc -c quicksort.c;
	gcc -c tree.c;
	gcc -Wall main.o quicksort.o tree.o -o main -lm;
	gcc -c main.c;
