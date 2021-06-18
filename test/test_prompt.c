#include "utils/test.h"

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

	i = 0;
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

int	main(int ac, char **av, char **env)
{
	(void)ac, (void)av;
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
		if (check_command(line, env))
			printf("%s\n", line);
		add_history(line);
		free(line);
		free(buff);
		buff = 0;
	}
	free(buff);
	return (0);
}
