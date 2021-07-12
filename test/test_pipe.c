#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "libft.h"

// gcc -Wall -Wextra -Werror -I ../header/ test_pipe.c -L . ../libs/libft.a

// test 1 - simple pipe
void	test_pipe1()
{
	char buffer[BUFSIZ + 1]; // bufsize est une macro de size gere par stdio

	int fd[2]; // tableau a 2 dimensions
	pipe(fd); // renvoie -1 en cas d'echec

	write(fd[1], "Hello hello\n", strlen("Hello hello\n")); // kernel stocke la donne ecrite
	// jusqu'a ce qu'elle soit lue par le read end du pipe
	read(fd[0], buffer, BUFSIZ);
	printf("%s", buffer);
	// NB si pas de EOF a l'un ou l'autre bout du pipe => pipe ne se stoppe pas?
}

// test 2 - simple ecriture parent -> lecture enfant
int	test_pipe2()
{
	int fd[2];
	int pid;
	char buffer[BUFSIZ];

	if (pipe(fd) == -1)
		return 0;
	pid = fork();
	if (pid == -1)
		return 0;
	if (pid == 0) // j'accede au processus enfant cree
	{
		close(fd[1]); // je ferme la partie ecriture enfant pour eviter les surprises
		while(read(fd[0], buffer, BUFSIZ) != 0) // je met en place le read - quand le parent ecrira c'est ici que je recevrais
			printf("dans le child = %s", buffer);
		close(fd[0]); // je ferme totalement le pipe enfant
		exit(1); // permet de quitter le processus de l'enfant
	}
	// else // par defaut je suis dans le processus pere
	{
		printf("pid parent = %d\n", getpid());
		close(fd[0]); // je ferme la partie read du parent
		write(fd[1], "Ce que j'écris dans le parent\n", strlen("Ce que j'écris dans le parent\n"));
		close(fd[1]); // je ferme le pipe parent
		pid = wait(NULL); // attend que le processus enfant soit nul + doit etre fait apres fermeture des fd sinon bloque parent
		printf("pid enfant qui vient de finir = %d\n", pid);
		// wait(&pid);
	}
	return 1;
}

int test_pipe3()
{
	int fd[2];
	int pid;

	if (pipe(fd) == -1)
		return 0;
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		printf("pid enfant = %d\n", pid);
		printf("ecriture child = %d\n", fd[1]);
		printf("lecture child = %d\n", fd[0]);
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		execlp("wc", "wc", "-l", NULL);
		exit(1);
		// wait(&pid);
		// pid =  wait(NULL);
		// return 1;
	}
	else
	{
		printf("pid parent = %d\n", getpid());
		printf("ecriture parent = %d\n", fd[1]);
		printf("lecture parent = %d\n", fd[0]);
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		execlp("ls", "ls", NULL);
		wait(NULL); // = idem que pid = wait(NULL)
		// wait(&pid);
		printf("pid enfant qui vient de finir = %d\n", pid);
		// return 1;
	}
	return 1;
}

// 1 - parse => fct check si token non literal est quoted
// 2 - parse => check si || cote a cote dans la liste => error
// 3 - si pipe => compter pipe et creer liste ptr des pipes dans liste chainee
// 4 - prendre premiere sublst avec pour end -> 1er pipe (et ainsi de suite) => obligation de creer une sublst de cmd ?
// => parser quote + var d'env
// exec


int	pipe_n_exec(char *cmd1, char *arg1, char *cmd2, char *arg2)
{
	int fd[2];
	int pid;
	
	if (pipe(fd) == -1)
		return 0;
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		printf("ecriture child = %d\n", fd[1]);
		printf("lecture child = %d\n", fd[0]);
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		execlp(cmd2, cmd2, arg2, NULL);
		return 1;
	}
	else
	{
		printf("pid parent = %d\n", getpid());
		printf("ecriture child = %d\n", fd[1]);
		printf("lecture child = %d\n", fd[0]);
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		execlp(cmd1, cmd1, arg1, NULL);
		return 1;
	}
	return 1;
}

int	test_pipe4()
{
	int count = 1;

	// char *cmd1 = "ls";
	// char *cmd2 = "wc";

	while(count > 0)
	{
		if (count == 1)
			pipe_n_exec("ls", NULL, "wc", "-l");
		// if (count == 2)
			// pipe_n_exec("ls", NULL, "cat", "-e");
		// if (count == 1)
			// pipe_n_exec("cat", "-e", "wc", NULL);
		count--;
	}
	return 1;
}


void	ft_del(void *content)
{
	free(content);
}

void	test_pipe5(t_dlist *list)
{
	int fd[2];
	int pid;
	int cfd;
	t_dlist *iter;

	cfd = 0;
	iter = list;
	while (iter)
	{
		pipe(fd); // je cree un pipe avec fd
		pid = fork(); // je cree un processus enfant
		if (pid == -1)
		{
			printf("fail\n");
			exit(1);
		}
		else if (pid == 0) // child
		{
			dup2(cfd, 0); // je copie cfd (par defaut fd read) dans stdin = entree
			if (iter->next != NULL)
				dup2(fd[1], 1); // je copie le fd write child dans stdout = sortie => redirige le flux vers stdout
			close(fd[0]); // je ferme le fd read child = fermeture de l'entree
			execlp(iter->content, iter->content, NULL); // cmd qui part dans stdout 
			exit (1); // je quitte le process child qd j'ai fini
		}
		else
		{
			wait(NULL); // j'attend que le child se termine
			close(fd[1]); // je ferme le fd read
			cfd = fd[0]; // cfd devient le fd read
			iter = iter->next;
		}
	}
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	// test_pipe1();
	// test_pipe2();
	// printf("pipe3\n\n");
	// test_pipe3();
	// printf("pipe4\n\n");
	// test_pipe4();

	t_dlist *list;
	list = NULL;

	dlstadd_back(&list , dlstnew(ft_strdup("echo")));
	dlstadd_back(&list , dlstnew(ft_strdup("ls")));
	// dlstadd_back(&list , dlstnew(ft_strdup("echo")));
	dlstadd_back(&list , dlstnew(ft_strdup("wc")));
	// t_dlist *iter;

	// iter = list;
	// while (iter)
	// {
	// 	printf("%s\n", iter->content);
	// 	iter = iter->next;
	// }

	test_pipe5(list);
	dlstclear(&list, &ft_del);


	return 0;
}