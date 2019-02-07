knn: main.o quicksort.o tree.o ui.o
	gcc -Wall -g`pkg-config --cflags gtk+-2.0` `pkg-config --cflags gthread-2.0` main.o quicksort.o tree.o ui.o -o $@ -lm `pkg-config --libs gtk+-2.0` `pkg-config --libs gthread-2.0`

main.o:
	gcc -c -Wall -g `pkg-config --cflags gtk+-2.0` `pkg-config --cflags gthread-2.0` main.c -o $@ -lm `pkg-config --libs gtk+-2.0` `pkg-config --libs gthread-2.0`

quicksort.o:
	gcc -c -Wall -g quicksort.c -o $@

tree.o:
	gcc -c -Wall -g tree.c -o $@

ui.o:
	gcc -c -Wall -g `pkg-config --cflags gtk+-2.0` `pkg-config --cflags gthread-2.0` ui-scrolled-canvas-skeleton.c -o $@ -lm `pkg-config --libs gtk+-2.0` `pkg-config --libs gthread-2.0`

.PHONY: clean clean_objects new

clean_objects:
	rm -f *.o

clean: clean_objects
	rm -f knn

new: clean knn