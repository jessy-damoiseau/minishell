/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jessy <jessy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:03:24 by jessy             #+#    #+#             */
/*   Updated: 2021/11/09 22:34:34 by jessy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_child(t_dlist *iter, int *fd, int cfd)
{
	if (cfd >= 0)
		dup2(cfd, 0);
	if (iter->next)
		dup2(fd[1], 1);
	expand_env();
	g_info.child = 1;
	exec_command(iter->content);
	close(fd[1]);
	close(fd[0]);
	exit(errno);
}

void	free_cmdpipe(t_dlist *list, int i, int *tabpid)
{
	t_dlist	*iter;
	int		status;

	status = 0;
	while (i >= 0)
		waitpid(tabpid[i--], &status, WUNTRACED);
	free(tabpid);
	check_if_exit(g_info.cmdpipe);
	g_info.stop = 0;
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
	g_info.cmdpipe = 0;
}

void	exec_parent(int *fd, int *cfd, t_dlist **iter, int *fdt)
{
	signal(SIGINT, SIG_IGN);
	close(fd[1]);
	(*cfd) = dup(fd[0]);
	(*iter) = (*iter)->next;
	g_info.child = 0;
	close(fd[0]);
	dup2(fdt[1], 1);
	dup2(fdt[0], 0);
}

void	exec_pipeline(t_dlist *iter, int *tabpid, int i, int cfd)
{
	int		fd[2];
	int		fdt[2];
	pid_t	pid;

	fdt[1] = dup(1);
	fdt[0] = dup(0);
	tabpid = malloc(sizeof(int) * dlstsize(iter));
	if (!tabpid)
		ft_exit(0, err_malloc);
	while (iter && !g_info.stop)
	{
		pipe(fd);
		pid = fork();
		tabpid[i++] = pid;
		if (pid == -1)
			ft_exit(0, err_pid);
		else if (pid == 0)
			exec_child(iter, fd, cfd);
		else
			exec_parent(fd, &cfd, &iter, fdt);
	}
	free_cmdpipe(g_info.cmdpipe, (i - 1), tabpid);
}
