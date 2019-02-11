#include "main.h"

#include "structures.h"

extern void quicksort (TREE array[], int start, int end, int TREE);
extern TREE *CreateTree(TREE kd_tree[], int start, int end, int TREE);

int treeSize = 0;
TREE *tree;

int main (int argc, char *argv[]){
    
	int line_size, bytes, counter;
	FILE *fd;
	char *numbers=NULL, ch[1];
	TREE *kd_tree=NULL;
	char *token;
	double x, y;
	//double distance=0;
	double distance[counter];

	
	unsigned long i;
//
	if (argc == 1)
	{
		printf("There is not a file.\n");
		return(1);
	}

	fd = fopen (argv[1], "r+");
	bytes = 1;
	counter = 0;
	while (bytes != 0) {

		line_size = 1;
		bytes = fread (ch, 1, 1, fd);

		if (strcmp(ch, "\n") == 0)
		{
			break;
		}

		numbers = (char *)calloc(1, sizeof(char));
		while (ch[0] != '\n')
		{
			numbers = (char *)realloc(numbers, line_size+1);
			numbers[strlen(numbers)] = ch[0];

			bytes = fread (ch, 1, 1, fd);
			if (bytes == 0)
			{
				break;
			}
			line_size++;
		}

		numbers[strlen(numbers)]='\0';

		kd_tree = (TREE *)realloc(kd_tree, (counter+1)*sizeof(TREE)+1);
		token = strtok(numbers, " ");
		sscanf(token, "%lf", &kd_tree[counter].x);
		while(token)
		{
			token = strtok(NULL, " ");
			if (token != NULL)
			{
			    sscanf(token, "%lf", &kd_tree[counter].y);
			}
		}

		counter++;
		free(numbers);
		numbers=NULL;
	}
	
	printf("The coordinates are:\n");
	for (i=0; i < counter; i++)
	{
		printf("(%.2lf, %.2lf)\n", kd_tree[i].x,kd_tree[i].y);
	}

	printf("\nThe coordinates after kd-tree are:\n");
	CreateTree(kd_tree, 0, counter-1, 0);

	find_maxes(kd_tree, counter);

	treeSize = counter;
	tree = kd_tree;
//

	HIST_ENTRY **hist_list;
	char dir[BUFFER_SIZE], command[BUFFER_SIZE];
	char symbol[BUFFER_SIZE + sizeof(NRM) + sizeof(CYN) + 4];
	char *text;
	char *hist_text;
	int expansion;
	int result;
	
	init_tcl(argv);

	init_readline();

	using_history();
	while (1)
	{
		getcwd(dir, sizeof(dir));
		sprintf(symbol,CYN"[%s]"NRM": ", dir);
		text = readline(symbol);
		if (text != NULL)
		{
			expansion = history_expand(text, &hist_text);
			switch (expansion)
			{
				case -1:	// An error occured.
					printf("Error in history expanding:\n\t%s\n", hist_text);
					exit(1);
				case 0:		// Expansion did not take place.
				case 2:		// Returned line should only be displayed, but not executed.
					add_history(text);
					strcpy(command, text);
					break;
				case 1:		// Expansion did take place
					add_history(hist_text);
					strcpy(command, hist_text);
					break;
				default:
					break;
			}
			free(hist_text);
			// Text is not freed, if I try to free it, it crashes.
			free(text);

			if (!strcmp(command, "quit"))
			{
				clear_history();
				Tcl_DeleteInterp(interpreter);
				exit(0);
			}
			// Personal history, not the Tcl Command
			else if (!strcmp(command, "history"))
			{
				hist_list = history_list();
				if (hist_list != NULL)
				{
					i = 0;
					// The history list is NULL terminated
					while (*(hist_list + i) != NULL)
					{
						printf("%ld:\t %s\n", (history_base + i), (*(hist_list + i))->line);
						i++;
					}
				}
			}
			else if (!strcmp(command, "start_gui"))
			{
				start_gui();
			}
			else
			{
				// Execute the command
				result = Tcl_Eval(interpreter, command);
				
				if (*Tcl_GetStringResult(interpreter) != '\0')
				{
					printf("%s\n", Tcl_GetStringResult(interpreter));
				}

				if (result == TCL_ERROR)
				{
					printf(RED"\tSomething is wrong with the command!\n"NRM);
				}
			}
		}
		// free(text);
	}

	// This Part is just the Eucledean distance a point to the stored coordinates //

	printf("\nGive the coordinates of the query point\n");
	printf("x: ");
	scanf("%lf", &x);
	printf("y: ");
	scanf("%lf", &y);

	//printf("%lf\n", kd_tree->right->y);
	for (i=0; i < counter; i++)
	{
		distance[i] = sqrt(pow((fabs(x - kd_tree[i].x)), 2) +  pow((fabs(y - kd_tree[i].y)), 2));
    	printf("The euclidean distance is: %lf\n", distance[i]);
	}


	/*distance = sqrt(pow((abs(x - ), 2) +  pow(abs((y - ), 2));

	printf("The euclidean distance is: %lf\n", distance);*/



	return 0;
}
