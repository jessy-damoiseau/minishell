#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

void	get_path2(char **path, char *buff, char *color, int i)
{
	int	k;
	int	j;

	k = 0;
	j = 0;
	while (color[k] != '|')
		(*path)[j++] = color[k++];
	(*path)[j++] = '~';
	while (buff[i])
		(*path)[j++] = buff[i++];
	(*path)[j++] = ' ';
	(*path)[j++] = '>';
	(*path)[j++] = '>';
	(*path)[j++] = ' ';
	while (color[++k])
		(*path)[j++] = color[k];
	(*path)[j] = '\0';
}

void	get_path(char **path, char *buff, char *color)
{
	int	i;
	int	nb_bslash;
	int	j;

	i = 0;
	nb_bslash = 0;
	while (buff[i] && nb_bslash < 3)
	{
		if (buff[i] == '/')
			nb_bslash++;
		i++;
	}
	i--;
	if (!((*path) = malloc(sizeof(char) * (strlen(buff) - i + 5
		+ strlen(color)))))
		return ;
	get_path2(path, buff, color, i);
}

int	main()
{
	char	*buff;
	char	*line;
	char	*path;
	int		i;

	buff = 0;
    while (1)
	{
		i = 13;
		while (!buff)
		{
			buff = malloc(sizeof(char) * i + 1);
			if (!(buff = getcwd(buff, i)))
				free(buff);
			i++;
		}
		get_path(&path, buff, "\033[1;31m|\033[0m");
    	if (!(line = readline(path)))
			break;
		printf("%s\n", line);
		free(line);
		free(buff);
		buff = 0;
	}
}