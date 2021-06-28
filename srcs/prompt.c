#include "minishell.h"

static void	ft_get_path2(char **path, char *buff, char *color, int i)
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

static int	ft_get_path(char **path, char *buff, char *color)
{
	int	i;

	i = 0;
	while (buff[i])
		i++;
	while (i > 0 && buff[i]!= '/')
		i--;
	i++;
	if (ft_go_malloc(path,
			(sizeof(char) * (strlen(buff) - i + 4 + strlen(color)))))
		return (0);
	ft_get_path2(path, buff, color, i);
	return (1);
}

int	ft_eof_signal(char *line, char *buff, t_info *info)
{
	(void)info;
	if (!line)
	{
		free(line);
		free(buff);
		ft_exit(info, no_err);
	}
	return (1);
}

void	ft_sighandler(int signum)
{
	if (signum == SIGINT)
		printf("check\n");
	return ;
}

void    ft_prompt(t_info *info)
{
	char	*buff;
	char	*line;
	char	*path;
	int		i;

	buff = NULL;
	while (1)
	{
		i = 13;
		while (!buff)
			buff = getcwd(buff, i++);
		if (!ft_get_path(&path, buff, "\033[1;35m|\033[0m"))
			ft_exit(info, err_malloc);
		line = readline(path);
		ft_eof_signal(line, buff, info);
		// add_history(line); // cree des leaks - non traite pour le moment
		ft_create_token(line, info);
		free(line);
		free(buff);
		buff = NULL;
	}
	return ;
}