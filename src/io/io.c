#include "io.h"

// *** parse_points_file *** //
// Parse a file containing points and store them. //
// -----------------------------------------------//
// The file elements have the following format:   //
//     <Name>  <Type>  <X-coord>  <Y-coord>       //
void parse_points_file(FILE *fp)
{
  char *line = NULL;           // The string of the line //
  char *token = NULL;          // Token //
  char *name = NULL;           // The name of the point //
  size_t length = 0;           // The length of the line //
  double x, y;                 // The x, y coordinates of the point //
  unsigned long nooflines = 0; // The number of lines of the file //

  // First Pass //
  // Get the number of lines of the file //
  while (getline(&line, &length, fp) != -1)
    {
      nooflines++;
    }

  // Initialize the array //
  init_element_array(nooflines);

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
      // Check if element's x coordinate exceed the max one //
      if (isgreater(x, elementmaxxcoordinate) == 1)
        {
          elementmaxxcoordinate = x;
        }

      // Third token - Y-coordinate //
      token = strtok(NULL, DELIMITERS);
      assert(token != NULL);

      y = atof(token);
      // Check if element's y coordinate exceed the max one //
      if (isgreater(y, elementmaxycoordinate) == 1)
        {
          elementmaxycoordinate = y;
        }

      // Syntax checking //
      token = strtok(NULL, DELIMITERS);
      assert(token == NULL);
      
      // Store point to the array //
      // The -1 indicates that the inserted element is a point with the following 0s its width and height //
      insert_element(name, x, y, -1, 0, 0);
    }

  // The last line needs to be freed //
  free(line);
}

// *** parse_drawbuffer_output_file *** //
// Parse the output file created from the ASP Tool and store its elements. //
// ------------------------------------------------------------------------//
// The file elements have the following format:                            //
//     <Name>  <Type>  <X-coord>  <Y-coord>  <Width>  <Height>             //
void parse_drawbuffer_output_file(FILE *fp)
{
  char *line = NULL;           // The string of the line //
  char *token = NULL;          // Token //
  char *name = NULL;           // The name of the element //
  size_t length = 0;           // The length of the line //
  int type;                    // The type of the element //
  double x, y;                 // The x & y coordinates of the element //
  double width, height;        // The width & height of the element //
  unsigned long nooflines = 0; // The number of lines of the file //

  // First Pass //
  // Get the number of lines of the file //
  while (getline(&line, &length, fp) != -1)
    {
      nooflines++;
    }

  // Initialize the array //
  init_element_array(nooflines);

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
      // Check if element's right side exceed the max x coordinate //
      if (isgreater((x + width), elementmaxxcoordinate) == 1)
        {
          elementmaxxcoordinate = x + width;
        }

      // Sixth token - Height //
      token = strtok(NULL, DELIMITERS);
      assert(token != NULL);

      height = atof(token);
      // Check if element's bottom side exceed the max y coordinate //
      if (isgreater((y + height), elementmaxycoordinate) == 1)
        {
          elementmaxycoordinate = y + height;
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
