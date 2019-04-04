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
OBJ = build/main.o build/tcl/tcl.o build/readline/readline.o build/ui/ui.o build/io/io.o build/structures/structures.o

knn: EXEC_HEADER := $(GUI_HEADER)
knn: EXEC_FOOTER := $(GUI_FOOTER) $(MATH) $(TCL) $(READLINE)
knn: $(OBJ)
	$(CC) $(CFLAGS) $(EXEC_HEADER) $^ -o $@ $(EXEC_FOOTER)

build/main.o: CFLAGS += -c
build/main.o: src/main.c src/main.h
	@ mkdir -p build
	$(CC) $(CFLAGS) $(HEADER) $< -o $@ $(FOOTER)

build/tcl/tcl.o: CFLAGS += -c
build/tcl/tcl.o: src/tcl/tcl.c src/tcl/tcl.h
	@ mkdir -p build/tcl
	$(CC) $(CFLAGS) $(HEADER) $< -o $@ $(FOOTER)

build/readline/readline.o: CFLAGS += -c
build/readline/readline.o: src/readline/readline.c src/readline/readline.h
	@ mkdir -p build/readline
	$(CC) $(CFLAGS) $(HEADER) $< -o $@ $(FOOTER)

build/ui/ui.o: CFLAGS += -c
build/ui/ui.o: HEADER := $(GUI_HEADER)
build/ui/ui.o: FOOTER := $(GUI_FOOTER)
build/ui/ui.o: src/ui/ui.c src/ui/ui.h
	@ mkdir -p build/ui
	$(CC) $(CFLAGS) $(HEADER) $< -o $@ $(FOOTER)

build/io/io.o: CFLAGS += -c
build/io/io.o: src/io/io.c src/io/io.h
	@ mkdir -p build/io
	$(CC) $(CFLAGS) $(HEADER) $< -o $@ $(FOOTER)

build/structures/structures.o: CFLAGS += -c
build/structures/structures.o: src/structures/structures.c src/structures/structures.h
	@ mkdir -p build/structures
	$(CC) $(CFLAGS) $(HEADER) $< -o $@ $(FOOTER)

.PHONY: clean clean_objects new

clean_objects:
	@ rm -f $(OBJ)
	@ rm -rf build/ build/tcl/ build/readline/ build/ui/ build/io/ build/structures/
	rm -rf build/

clean: clean_objects
	rm -f knn

new: clean knn