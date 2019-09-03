#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

void init_readline();

char **custom_completion(const char *, int, int);

char *custom_completer(const char * , int);