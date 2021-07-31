#include "minishell.h"

void    create_pipeline(t_info *info, int pipe)
{
	(void)info;
	(void)pipe;
	// ici creer un tableau de pointeur de dlist et y decouper la list cmd entre chaque pipe
	return ;
}

void	exec_child(t_dlist *iter, int *fd, int cfd)
{
	dup2(cfd, 0);
	if (iter->next != NULL)
		dup2(fd[1], 1);
	close(fd[0]);
	close(fd[1]);
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
			ft_exit(0, info, err_malloc); // code err a modifier ou adapter quand @Jessy aura cree exit
		else if (pid == 0)
			exec_child(iter, fd, cfd);
		else
		{
			wait(NULL);
			close(fd[1]);
			cfd = fd[0];
			iter = iter->next; // @Jessy ici passage au prochain pipe
		}
	}
}