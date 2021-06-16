#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

int	go_malloc(char **str, int len)
{
	(*str) = malloc(len);
	if (!str)
		return (1);
	return (0);
}


void	get_path2(char **path, char *buff, char *color, int i)
{
	int	k;
	int	j;

	k = 0;
	j = 0;
	while (color[k] != '|')
		(*path)[j++] = color[k++];
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

	i = 0;
	nb_bslash = 0;
	while (buff[i])
		i++;
	while (i > 0 && buff[i]!= '/')
		i--;
	i++;
	if (go_malloc(path,
			(sizeof(char) * (strlen(buff) - i + 4 + strlen(color)))))
		return ;
	get_path2(path, buff, color, i);
}

int	main(void)
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
			buff = getcwd(buff, i++);
		get_path(&path, buff, "\033[1;35m|\033[0m");
		line = readline(path);
		if (!line)
		{
			printf("\n");
			break ;
		}
		printf("%s\n", line);
		free(line);
		free(buff);
		buff = 0;
	}
	free(buff);
	return (0);
}
