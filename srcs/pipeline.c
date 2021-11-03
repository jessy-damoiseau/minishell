/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:03:24 by jessy             #+#    #+#             */
/*   Updated: 2021/11/02 23:08:52 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_child(t_dlist *iter, int *fd, int cfd)
{
	signal(SIGINT, SIG_IGN);
	if (cfd >= 0)
		dup2(cfd, 0);
	if (iter)
		dup2(fd[1], 1);
	expand_env();
	g_info.child = 1;
	exec_command();
	exit (errno);
}

void	free_cmdpipe(t_dlist *list)
{
	t_dlist	*iter;

	while (list)
	{
		iter = list;
		if (iter->content != 0)
		{
			iter = list->content;
			clear_cmd_lst(&iter);
			iter = list;
		}
		list = list->next;
		free(iter);
	}
}

void	exec_parent(int *fd, int *cfd, t_dlist **iter, int *fdt)
{
	wait(0);
	close(fd[1]);
	(*cfd) = dup(fd[0]);
	(*iter) = (*iter)->next;
	g_info.cmdpipe = g_info.cmdpipe->next;
	g_info.child = 0;
	close(fd[0]);
	dup2(fdt[1], 1);
	dup2(fdt[0], 0);
}

void	exec_pipeline(t_dlist *list)
{
	int		fd[2];
	int		fdt[2];
	int		pid;
	int		cfd;
	t_dlist	*iter;

	cfd = -1;
	iter = list;
	fdt[1] = dup(1);
	fdt[0] = dup(0);
	while (iter && !g_info.stop)
	{
		pipe(fd);
		pid = fork();
		if (pid == -1)
			ft_exit(0, err_pid);
		else if (pid == 0)
			exec_child(iter->next, fd, cfd);
		else
			exec_parent(fd, &cfd, &iter, fdt);
	}
	g_info.stop = 0;
	free_cmdpipe(list);
	g_info.cmdpipe = 0;
}
