#include "io.h"

void parse_points_file(FILE *fp)
{
	char *line = NULL;
	char *token = NULL;
	char *name = NULL;
	size_t length = 0;
	double x, y;

	token = strtok(line, DELIMITERS);

	while (getline(&line, &length, fp) != -1)
	{
		// First token - Point Name //
		token = strtok(line, DELIMITERS);
		if (token == NULL)
		{
			printf(RED"Syntax Error!\n"NRM);
			exit(1);
		}
		name = strdup(token);

		// Second token - X-coordinate //
		token = strtok(NULL, DELIMITERS);
		if (token == NULL)
		{
			printf(RED"Syntax Error!\n"NRM);
			exit(1);
		}
		x = atof(token);

		// Third token - Y-coordinate //
		token = strtok(NULL, DELIMITERS);
		if (token == NULL)
		{
			printf(RED"Syntax Error!\n"NRM);
			exit(1);
		}
		y = atof(token);

		// Syntax checking //
		token = strtok(NULL, DELIMITERS);
		if (token != NULL)
		{
			printf(RED"Syntax Error!\n"NRM);
			exit(1);
		}

		// Store point to the hashtable //
		insert_point(name, x, y);
	}

	free(line);
}