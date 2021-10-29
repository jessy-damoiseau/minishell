#include "minishell.h"

void	create_pipeline(void)
{
	t_dlist *lr;
	t_dlist	*tmp;
	t_dlist	*pipe;
	t_token	*token;

	pipe = g_info.cmd;
	while (pipe->next)
	{
		token = pipe->content;
		if (token->type == pipeline)
		{
			lr = pipe->prev;
			token = lr->content;
			while (token->type == space)
			{
				lr = lr->prev;
				token = lr->content;
			}
			pipe = pipe->next;
			token = pipe->content;
			while (token->type == space)
			{
				pipe = pipe->next;
				token = pipe->content;
			}
			tmp = pipe->prev;
			tmp->next = 0;
			pipe->prev = 0;
			tmp = lr->next;
			tmp->prev = 0;
			clear_cmd_lst(&tmp);
			lr->next = 0;
			while (lr->prev)
				lr = lr->prev;
			dlstadd_back(&g_info.cmdpipe, dlstnew(lr));
		}
		else
			pipe = pipe->next;
	}
	while (pipe->prev)
		pipe = pipe->prev;
	dlstadd_back(&g_info.cmdpipe, dlstnew(pipe));
	

// *test* //
	// pipe = g_info.cmdpipe;
	// int i = 0;
	// while (pipe)
	// {
	// 	printf("%d ->:\n", i++);
	// 	t_dlist *tmpm;
	// 	tmpm = pipe->content;
	// 	printf("lstsize:|%d|\n", dlstsize(tmpm));
	// 	while (tmpm)
	// 	{
	// 		token = tmpm->content;
	// 		printf("|%s|\n", (char*)token->value);
	// 		tmpm = tmpm->next;
	// 	}
	// 	pipe = pipe->next;
	// }
	// printf("nbpipe:|%d|\n", g_info.nbpipe);
// *end_test* //

	exec_pipeline(g_info.cmdpipe);
}


void	exec_child(t_dlist *iter, int *fd, int cfd)
{
	if (cfd >= 0)
		dup2(cfd, 0);
	if(iter)
		dup2(fd[1], 1);
	expand_env();
	g_info.child = 1;
	exec_command();
	exit (errno);
}

// @Jessy ici remplacer list par la structure / tableau de list qui te convient 
void	exec_pipeline(t_dlist *list)
{
	int fd[2];
	int pid;
	int cfd;
	int fd1;
	int fd0;
	t_dlist *iter;

	cfd = -1;
	iter = list;
	fd1 = dup(1);
	fd0 = dup(0);
	while (iter)
	{
		pipe(fd);
		pid = fork();
		if (pid == -1)
			ft_exit(0, err_pid);
		else if (pid == 0)
		{
			exec_child(iter->next, fd, cfd);
		}
		else
		{
			wait(0);
			close(fd[1]);
			cfd = dup(fd[0]);
			iter = iter->next;
			g_info.cmdpipe = g_info.cmdpipe->next;
			g_info.child = 0;
			close(fd[0]);
			dup2(fd1, 1);
			dup2(fd0, 0);
		}
	}
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
	dup2(fd1, 1);
	dup2(fd0, 0);
}