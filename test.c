#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

int main()
{
	char *line = 0;
    while (1)
    	if (!(line = readline("\033[1;31mminishell->\033[0m")))
			break;
		printf("%s\n", line);
		free(line);
}