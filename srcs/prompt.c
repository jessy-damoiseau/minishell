#include "minishell.h"

void	ft_sighandler(int signum)
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

void	ft_prompt(t_info *info)
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
