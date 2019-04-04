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
		assert(token != NULL);

		name = strdup(token);

		// Second token - X-coordinate //
		token = strtok(NULL, DELIMITERS);
		assert(token != NULL);

		x = atof(token);
		if (isgreater(x, pointMaxXCoordinate) == 1)
		{
			pointMaxXCoordinate = x;
		}

		// Third token - Y-coordinate //
		token = strtok(NULL, DELIMITERS);
		assert(token != NULL);

		y = atof(token);
		if (isgreater(y, pointMaxYCoordinate) == 1)
		{
			pointMaxYCoordinate = y;
		}

		// Syntax checking //
		token = strtok(NULL, DELIMITERS);
		assert(token == NULL);
		
		// Store point to the hashtable //
		insert_point(name, x, y);
	}

	free(line);
}