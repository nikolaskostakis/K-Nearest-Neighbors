# Compiler and compiler flags #
CC = gcc
CFLAGS = -g -Wall

# Link Libraries #
MATH = -lm
TCL = -ltcl
READLINE = -lreadline

# Headers #
EXEC_HEADER =
HEADER =
GUI_HEADER = `pkg-config --cflags gtk+-2.0` `pkg-config --cflags gthread-2.0`

# Footers #
EXEC_FOOTER =
FOOTER =
GUI_FOOTER = `pkg-config --libs gtk+-2.0` `pkg-config --libs gthread-2.0`

# Objects #
OBJ = main.o ui.o tcl.o readline.o io.o structures.o

knn: EXEC_HEADER := $(GUI_HEADER)
knn: EXEC_FOOTER := $(GUI_FOOTER) $(MATH) $(TCL) $(READLINE)
knn: $(OBJ)
	$(CC) $(CFLAGS) $(EXEC_HEADER) $^ -o $@ $(EXEC_FOOTER)

main.o: CFLAGS += -c
main.o: src/main.c src/main.h
	$(CC) $(CFLAGS) $(HEADER) $< -o $@ $(FOOTER)

tcl.o: CFLAGS += -c
tcl.o: src/tcl/tcl.c src/tcl/tcl.h
	$(CC) $(CFLAGS) $(HEADER) $< -o $@ $(FOOTER)

readline.o: CFLAGS += -c
readline.o: src/readline/readline.c src/readline/readline.h
	$(CC) $(CFLAGS) $(HEADER) $< -o $@ $(FOOTER)

ui.o: CFLAGS += -c
ui.o: HEADER := $(GUI_HEADER)
ui.o: FOOTER := $(GUI_FOOTER)
ui.o: src/ui/ui-scrolled-canvas-skeleton.c src/ui/ui-scrolled-canvas-skeleton.h
	$(CC) $(CFLAGS) $(HEADER) $< -o $@ $(FOOTER)

io.o: CFLAGS += -c
io.o: src/io/io.c src/io/io.h
	$(CC) $(CFLAGS) $(HEADER) $< -o $@ $(FOOTER)

structures.o: CFLAGS += -c
structures.o: src/structures/structures.c src/structures/structures.h
	$(CC) $(CFLAGS) $(HEADER) $< -o $@ $(FOOTER)

.PHONY: clean clean_objects new

clean_objects:
	rm -f *.o

clean: clean_objects
	rm -f knn

new: clean knn