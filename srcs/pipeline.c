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
	//t_dlist *tmp;
	
	// dup2(cfd, 0);
	// if (iter->next != NULL)
	// 	dup2(fd[1], 1);
	// close(fd[0]);
	// close(fd[1]);
	(void)iter;
	if (cfd >= 0)
	{
		//printf("check1\n");
		dup2(cfd, 0);
	}
	if(iter)
	{	
		//printf("check\n");
		dup2(fd[1], 1);
	}
	expand_env(info);
	exec_command(info);
	// expand_n_exec() // @Jessy ici il faut expand la var d'env dans le pipe puis exec cmd
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
	t_dlist	*tmp;
	int i = 0;

	cfd = -1;
	iter = list;
	tmp = list;
	(void)tmp;
	fd1 = dup(1);
	fd0 = dup(0);
	while (iter)
	{
		//t_token *t;
		//t_dlist *p;
		//p = iter->content;
		//t = p->content;
		//printf("iter->content: %s\n", (char*)t->value);
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
			close(fd[0]);
			dup2(fd1, 1);
			dup2(fd0, 0);// @Jessy ici passage au prochain pipe
		}
	}
	dup2(fd1, 1);
	dup2(fd0, 0);
	//clear_cmdpipe;
}