#include "main.h"

int main (int argc, char *argv[]){
	unsigned long i;
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
		// sleep(1);
		getcwd(dir, sizeof(dir));
		sprintf(symbol,CYN"[%s]"NRM": ", dir);
		text = readline(symbol);
		// sleep(2);
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
	
	return 0;
}
