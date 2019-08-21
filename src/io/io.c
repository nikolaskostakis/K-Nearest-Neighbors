#include "io.h"

// *** parse_points_file *** //
// Parse through the file that has the points and store them //
void parse_points_file(FILE *fp)
{
  char *line = NULL;           // The string of the line //
  char *token = NULL;          // Token //
  char *name = NULL;           // The name of the point //
  size_t length = 0;           // The length of the line //
  double x, y;                 // The x, y coordinates of the point //
  unsigned long noofLines = 0; // The number of lines of the file //

  // First Pass //
  // Get the number of lines of the file //
  while (getline(&line, &length, fp) != -1)
    {
      noofLines++;
    }

  // Initialize the array //
  init_element_array(noofLines);

  // Revert the file pointer to the start of the file //
  rewind(fp);

  // Second Pass //
  // Get the points from the file //

  // While the file is not empty, get a line //
  while (getline(&line, &length, fp) != -1)
    {
      // Break the line into tokens //

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
      
      // Store point to the array //
      insert_point(name, x, y);
    }

  // The last line needs to be freed //
  free(line);
}

void parse_drawbuffer_output_file(FILE *fp)
{
  char *line = NULL;           // The string of the line //
  char *token = NULL;          // Token //
  char *name = NULL;           // The name of the element //
  size_t length = 0;           // The length of the line //
  int type;                    // The type of the element //
  double x, y;                 // The x & y coordinates of the element //
  double width, height;        // The width & height of the element //
  unsigned long noofLines = 0; // The number of lines of the file //

  // First Pass //
  // Get the number of lines of the file //
  while (getline(&line, &length, fp) != -1)
    {
      noofLines++;
    }

  // Initialize the array //
  init_element_array(noofLines);

  // Revert the file pointer to the start of the file //
  rewind(fp);

  // Second Pass //
  // Get the elements from the file //

  // While the file is not empty, get a line //
  while (getline(&line, &length, fp) != -1)
    {
      // Break the line into tokens //

      // First token - Rectangle Name //
      token = strtok(line, DELIMITERS);
      assert(token != NULL);

      name = strdup(token);

      // Second token - Type //
      token = strtok(NULL, DELIMITERS);
      assert(token != NULL);

      type = atoi(token);

      // Third token - X-coordinate //
      token = strtok(NULL, DELIMITERS);
      assert(token != NULL);

      x = atof(token);

      // Fourth token - Y-coordinate //
      token = strtok(NULL, DELIMITERS);
      assert(token != NULL);

      y = atof(token);

      // Fifth token - Width //
      token = strtok(NULL, DELIMITERS);
      assert(token != NULL);

      width = atof(token);
      if (isgreater((x + width), pointMaxXCoordinate) == 1)
        {
          pointMaxXCoordinate = x + width;
        }

      // Sixth token - Height //
      token = strtok(NULL, DELIMITERS);
      assert(token != NULL);

      height = atof(token);
      if (isgreater((y + height), pointMaxYCoordinate) == 1)
        {
          pointMaxYCoordinate = y + height;
        }

      // Syntax checking //
      token = strtok(NULL, DELIMITERS);
      assert(token == NULL);
      
      // Store point to the array //
      insert_element(name, x, y, type, width, height);
    }

  // The last line needs to be freed //
  free(line);
}
