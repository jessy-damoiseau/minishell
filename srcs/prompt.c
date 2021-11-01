/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 23:50:13 by pgueugno          #+#    #+#             */
/*   Updated: 2021/11/01 15:53:06 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_sighandler(int signum)
{
	if (signum == SIGINT)
	{	
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
		g_info.stop = 130;
		g_info.tmperrno = 130;
		errno = 130;
	}
}

void	refill_pwd(void)
{
	int		i;
	t_list	*tmp;
	char	*str;

	i = 13;
	str = 0;
	while (!str && i < 1000)
		str = getcwd(str, i++);
	if (str)
	{
		free(g_info.pwd);
		g_info.pwd = str;
	}
	tmp = g_info.env;
	while (tmp && ft_strncmp(tmp->content, "PWD=", 4))
		tmp = tmp->next;
	free(tmp->content);
	tmp->content = ft_strjoin("PWD=", g_info.pwd);
}

void	ft_prompt(void)
{
	char	*line;

	while (1)
	{
		init_var();
		g_info.tmperrno = errno;
		refill_pwd();
		line = get_prompt();
		errno = g_info.tmperrno;
		if (line[0])
		{
			add_history(line);
			ft_create_token(line);
			if (!g_info.nbpipe)
				exec_command();
		}
		free(line);
	}
	return ;
}
