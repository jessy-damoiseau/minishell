/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jessy <jessy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 23:50:13 by pgueugno          #+#    #+#             */
/*   Updated: 2021/10/28 19:33:16 by jessy            ###   ########.fr       */
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
		info.stop = 130;
	}
}

void	refill_pwd(void)
{
	int		i;
	t_list	*tmp;

	i = 13;
	while (!info.pwd)
		info.pwd = getcwd(info.pwd, i++);
	tmp = info.env;
	while (tmp && ft_strncmp(tmp->content, "PWD=", 4))
		tmp = tmp->next;
	free(tmp->content);
	tmp->content = ft_strjoin("PWD=", info.pwd);
}

void	ft_prompt(void)
{
	char	*line;

	while (1)
	{
		init_var();
		info.tmperrno = errno;
		refill_pwd();
		line = get_prompt();
		errno = info.tmperrno;
		if (line[0])
		{
			add_history(line);
			ft_create_token(line);
			if (!info.nbpipe)
				exec_command();
		}
		free(line);
		free(info.pwd);
		info.pwd = 0;
	}
	return ;
}
