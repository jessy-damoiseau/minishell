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
	(void)info;
	dup2(cfd, 0);
	if (iter->next != NULL)
		dup2(fd[1], 1);
	close(fd[0]);
	close(fd[1]);
	expand_env(info);
	exec_command(info);
	// expand_n_exec() // @Jessy ici il faut expand la var d'env dans le pipe puis exec cmd
	//exit (1);
}

// @Jessy ici remplacer list par la structure / tableau de list qui te convient 
void	exec_pipeline(t_dlist *list, t_info *info)
{
	int fd[2];
	int pid;
	int cfd;
	t_dlist *iter;

	cfd = 0;
	iter = list;
	while (iter)
	{
		pipe(fd);
		pid = fork();
		if (pid == -1)
			ft_exit(0, info, err_pid);
		else if (pid == 0)
			exec_child(info, iter, fd, cfd);
		else
		{
			wait(NULL);
			close(fd[1]);
			cfd = fd[0];
			iter = iter->next; // @Jessy ici passage au prochain pipe
		}
	}
}