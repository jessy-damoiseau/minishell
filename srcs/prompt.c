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

int			catch_eof_signal(char *line, char *path, t_info *info)
{
	if (!line)
	{
		free(line);
		free(path);
		ft_exit(0, info, no_err);
	}
	return (1);
}

void		ft_sighandler(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	refill_pwd(t_info *info)
{
	int		i;
	t_list	*tmp;

	i = 13;
	while (!info->pwd)
		info->pwd = getcwd(info->pwd, i++);
	tmp = info->env;
	while (tmp && ft_strncmp(tmp->content, "PWD=", 4))
	tmp = tmp->next;
	free(tmp->content);
	tmp->content = ft_strjoin("PWD=", info->pwd);
}

char	*get_prompt(t_info *info)
{
	char	*path;
	char 	*tmp;
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

void		ft_prompt(t_info *info)
{
	char	*line;
	
	while (1)
	{
		init_var(info);
		info->tmperrno = errno;
		refill_pwd(info);
		line = get_prompt(info);
		errno = info->tmperrno;
		if (line[0])
		{
			add_history(line);
			ft_create_token(line, info);
			if (!info->nbpipe)
				exec_command(info);
		}
		free(line);
		free(info->pwd);
		info->pwd = 0;
	}
	return ;
}