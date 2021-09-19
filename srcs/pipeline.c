#include "minishell.h"

void    create_pipeline(t_info *info, int pipe) // a refaire
{
	(void)info;
	(void)pipe;
	
	t_dlist *tmp1;
	t_dlist *tmp2;
	t_token *token;
	info->cmdpipe = 0;
	while (info->cmd->next)
	{
		token = info->cmd->content;
		if (!token->type)
		{
			tmp1 = info->cmd->prev;
			tmp1->next = 0;
			info->cmd->prev = 0;
			while (tmp1->prev)
				tmp1 = tmp1->prev;
			check_end((t_dlist**)&tmp1);
			dlstadd_back(&info->cmdpipe, dlstnew(tmp1));
			tmp2 = info->cmd;
			info->cmd = info->cmd->next;
			info->cmd->prev = 0;
			tmp2->next = 0;
			clear_cmd_lst(&tmp2);
			token = info->cmd->content;
			while (token->type == 2)
			{
				info->cmd = info->cmd->next;
				token = info->cmd->content;
			}
			if (info->cmd->prev)
			{
				tmp2 = info->cmd->prev;
				info->cmd->prev = 0;
				tmp2->next = 0;
				while (tmp2->prev)
					tmp2 = tmp2->prev;
				clear_cmd_lst(&tmp2);
			}
		}
		else
			info->cmd = info->cmd->next;
	}
	while (info->cmd->prev)
		info->cmd = info->cmd->prev;
	check_end((t_dlist**)&info->cmd);
	dlstadd_back(&info->cmdpipe, dlstnew(info->cmd));
	clear_cmd_lst(&info->cmd);

	int i = 0;
	while (info->cmdpipe)
	{
		i++;
		info->cmdpipe = info->cmdpipe->next;
	}
	exec_pipeline(info->cmdpipe, info);
	
	return ;
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
	// expand_env(info);
	// exec_command(info);
	// tmp = info->cmdpipe;
	// info->cmdpipe = info->cmdpipe->next;
	// tmp->next = 0;
	// info->cmdpipe->prev = 0;
	// clear_cmd_lst(&tmp);
	// expand_n_exec() // @Jessy ici il faut expand la var d'env dans le pipe puis exec cmd
	exit (1);
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