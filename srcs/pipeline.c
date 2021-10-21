#include "minishell.h"

void	supp_pipe(t_dlist **pipe)
{
	t_token	*token;
	token = (*pipe)->content;
	free(token->value);
	free(token);
	free(*pipe);
	*pipe = 0;
}

void	create_pipeline(t_info *info)
{
	t_dlist	*tmpcmd;
	t_dlist	*tmp;
	t_token	*token;

	tmpcmd = info->cmd;
	info->cmdpipe = 0;
	while (tmpcmd->next)
	{
		token = tmpcmd->content;
		if (token->type == pipeline)
		{
			tmp = tmpcmd;
			tmpcmd = tmpcmd->next;
			token = tmpcmd->content;
			while (token->type == space)
			{
				tmpcmd = tmpcmd->next;
				token = tmpcmd->content;
				supp_pipe(&tmpcmd->prev);
			}
			tmpcmd->prev = 0;
			tmp = tmp->prev;

			supp_pipe(&tmp->next);
			while (tmp->prev)
				tmp = tmp->prev;
			check_end((t_dlist **)&tmp);
			dlstadd_back(&info->cmdpipe, dlstnew(tmp));
		}
		else
			tmpcmd = tmpcmd->next;
	}
	while (tmpcmd->prev)
		tmpcmd = tmpcmd->prev;
	dlstadd_back(&info->cmdpipe, dlstnew(tmpcmd));
	

// *test* //
	// tmpcmd = info->cmdpipe;
	// int i = 0;
	// while (tmpcmd)
	// {
	// 	printf("%d ->:\n", i++);
	// 	t_dlist *tmpm;
	// 	tmpm = tmpcmd->content;
	// 	while (tmpm)
	// 	{
	// 		token = tmpm->content;
	// 		printf("|%s|\n", (char*)token->value);
	// 		tmpm = tmpm->next;
	// 	}
	// 	tmpcmd = tmpcmd->next;
	// }
	// printf("nbpipe:|%d|\n", info->nbpipe);
// *end_test* //

	exec_pipeline(info->cmdpipe, info);
}


void	exec_child(t_info *info, t_dlist *iter, int *fd, int cfd)
{
	if (cfd >= 0)
		dup2(cfd, 0);
	if(iter)
		dup2(fd[1], 1);
	expand_env(info);
	info->child = 1;
	exec_command(info);
	exit (errno);
}

// @Jessy ici remplacer list par la structure / tableau de list qui te convient 
void	exec_pipeline(t_dlist *list, t_info *info)
{
	int fd[2];
	int pid;
	int cfd;
	int fd1;
	int fd0;
	t_dlist *iter;
	int i = 0;

	cfd = -1;
	iter = list;
	fd1 = dup(1);
	fd0 = dup(0);
	while (iter)
	{
		pipe(fd);
		pid = fork();
		if (pid == -1)
			ft_exit(0, info, err_pid);
		else if (pid == 0)
		{
			exec_child(info, iter->next, fd, cfd);
		}
		else
		{
			i++;
			wait(NULL);
			close(fd[1]);
			cfd = dup(fd[0]);
			iter = iter->next;
			info->cmdpipe = info->cmdpipe->next;
			info->child = 0;
			close(fd[0]);
			dup2(fd1, 1);
			dup2(fd0, 0);
		}
	}
	dup2(fd1, 1);
	dup2(fd0, 0);
}