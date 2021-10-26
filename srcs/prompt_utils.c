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
	while (i > 0 && buff[i] != '/')
		i--;
	i++;
	if (ft_go_malloc(path,
			(sizeof(char) * (strlen(buff) - i + 4 + strlen(color)))))
		return (0);
	ft_get_path2(path, buff, color, i);
	return (1);
}

int	catch_eof_signal(char *line, char *path, t_info *info)
{
	if (!line)
	{
		free(line);
		free(path);
		ft_exit(0, info, no_err);
	}
	return (1);
}

char	*get_prompt(t_info *info)
{
	char	*path;
	char	*tmp;
	char	*ret;

	if (!ft_get_path(&path, info->pwd, "\033[1;35m|\033[0m"))
		ft_exit(0, info, err_malloc);
	tmp = readline(path);
	catch_eof_signal(tmp, path, info);
	ret = ft_strtrim(tmp, " ");
	free(tmp);
	free(path);
	return (ret);
}
