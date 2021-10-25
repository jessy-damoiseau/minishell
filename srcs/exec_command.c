#include "minishell.h"

void	check_end(t_dlist **alst)
{
	t_token *token;
	t_dlist *tmp1;
	t_dlist *tmp2;

	tmp1 = *alst;
	if (!tmp1->next)
		return ;
	while (tmp1 && tmp1->next->next)
		tmp1 = tmp1->next;
	tmp2 = tmp1->next;
	token = tmp2->content;
	if (!ft_strcmp((char *)token->value, " "))
	{
		free(token->value);
		free(token);
		free(tmp2);
		tmp1->next = 0;
	}
}

t_dlist	*fill_gbc(t_dlist *lst, t_info *info, int i)
{
	t_token *token;
	t_dlist *tmp;
	t_dlist *forgbc;
	int check;

	token = lst->content;
	tmp = lst;
	check = 0;
	if (!i)
	{
		while (tmp && (!(token->type >= 3 && token->type <= 6)))
		{
			tmp = tmp->next;
			if (tmp)
				token = tmp->content;
			i++;
			check = 1;
		}
	}
	else 
		check = 1;
	tmp = lst;
	forgbc = tmp;
	if (check)
	{
		while (i-- > 1)
			tmp = tmp->next;
		lst = tmp->next;
		tmp->next = 0;
		check_end((t_dlist **)&forgbc);
		lstaddback_gbc(&info->gbc, newgbc(TOKEN, -1, (void *)forgbc));
	}
	return (lst);
}

int		check_path(char *tmp1, t_info *info)
{
	char	*env_path;
	char	**path;
	char	*checkpth;
	t_list	*tmp;
	struct stat sb;
	int		i;

	i = 0;
	tmp = info->env;
	while (tmp && ft_strncmp(tmp->content, "PATH=", 5))
		tmp = tmp->next;
	env_path = tmp->content;
	path = ft_split(&env_path[5], ':');
	while (path[i++])
	{
		checkpth = strpthjoin(path[i], tmp1);
		if (!stat(checkpth, &sb))
		{
			ft_lstadd_back(&info->path, ft_lstnew((void *)checkpth));
			free_dbl(path);
			return (0);
		}
		free(checkpth);
	}
	free_dbl(path);
	return (1);
}

int		check_exist(char *str)
{
	struct stat sb;

	if (stat(str, &sb))
		return (1);
	return (0);
}

int		check_binaire(char *str)
{
	int	fd;
	char	s[5];

	fd = open(str, O_RDONLY);
	if (fd < 0)
		return (1);
	read(fd, &s[0], 4);
	s[4] = '\0';
	if (!ft_strcmp(&s[1], "ELF"))
		return (close(fd));
	return (close(fd) + 1);
	
}

int		check_command(t_info *info)
{
	t_token	*token1;
	t_token	*token2;
	t_dlist	*dlsttmp1;
	t_dlist	*dlsttmp2;
	char	*chrtmp1;
	int i;

	if (info->nbpipe)
		dlsttmp1 = info->cmdpipe->content;
	else
		dlsttmp1 = info->cmd;
	
	while (dlsttmp1->next)
		dlsttmp1 = dlsttmp1->next;
	token1 = dlsttmp1->content;
	if (token1->type >= 3 && token1->type <= 6)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
		clear_cmd_lst(&info->cmd);
		errno = 1;
		return (1);
	}
	if (token1->type == 2)
	{
		while (token1->type == 2)
		{
			dlsttmp1 = dlsttmp1->prev;
			token1  = dlsttmp1->content;
		}
		if (token1->type >= 3 && token1->type <= 6)
		{
			ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
			clear_cmd_lst(&info->cmd);
			errno = 1;
			return (1);
		}
	}
	if (info->nbpipe)
		dlsttmp1 = info->cmdpipe->content;
	else
		dlsttmp1 = info->cmd;
	while (dlsttmp1)
	{
		i = 2;
		token1 = dlsttmp1->content;
		if (token1->type >= 3 && token1->type <= 6)
		{
			token2 = dlsttmp1->next->content;
			dlsttmp2 = dlsttmp1->next;
			while (dlsttmp2 && token2->type != 9)
			{
				dlsttmp2 = dlsttmp2->next;
				if (dlsttmp2)
				{
					token2 = dlsttmp2->content;
					i++;
				}
			}
			dlsttmp1 = fill_gbc(dlsttmp1, info, i);
		}
		else
		{
			chrtmp1 = token1->value;
			if (!ft_strcmp(chrtmp1, "echo")
			|| !ft_strcmp(chrtmp1, "cd")
			|| !ft_strcmp(chrtmp1, "pwd")
			|| !ft_strcmp(chrtmp1, "export")
			|| !ft_strcmp(chrtmp1, "unset")
			|| !ft_strcmp(chrtmp1, "env")
			|| !ft_strcmp(chrtmp1, "exit"))
				dlsttmp1 = fill_gbc(dlsttmp1, info, 0);
			else
			{
				if (!check_exist(chrtmp1) && !check_binaire(chrtmp1))
				{
					dlsttmp1 = fill_gbc(dlsttmp1, info, 0);
				}
				else if (((chrtmp1[0] == '.' && chrtmp1[1] == '/') || chrtmp1[0] == '/'))
				{
					ft_putstr_fd("bash: ", 2);
					ft_putstr_fd(chrtmp1, 2);
					ft_putstr_fd(": No such file or directory\n", 2);
					clear_cmd_lst(&info->cmd);
					errno = 127;
					return (1);
				}
				else if (!check_path(chrtmp1, info) || (info->gbc && token1->type != 2))
					dlsttmp1 = fill_gbc(dlsttmp1, info, 0);
				else if (!ft_strcmp(chrtmp1, " "))
				{
					dlsttmp2 = dlsttmp1;
					dlsttmp1 = dlsttmp1->next;
					free(dlsttmp2);
				}
				else
				{
					ft_putstr_fd(chrtmp1, 2);
					ft_putstr_fd(": command not found\n", 2);
					clear_cmd_lst(&info->cmd);
					errno = 127;
					return (1);
				}
			}
		}
	}
	return (0);
}

void	joincmd(char **cmd, t_dlist *lst)
{
	t_token *tmp;
	t_dlist *tmp1;
	int i;
	int j;

	i = 0;
	j = 0;
	tmp1 = lst;
	while (tmp1)
	{
		tmp = tmp1->content;
		i += ft_strlen((char *)tmp->value);
		tmp1 = tmp1->next;
	}
	if (ft_go_malloc(cmd, (sizeof(char) * i + 1)))
		return ;
	tmp1 = lst;
	while (tmp1)
	{
		i = 0;
		tmp = tmp1->content;
		while (((char*)tmp->value)[i])
			(*cmd)[j++] = ((char *)tmp->value)[i++];
		tmp1 = tmp1->next;
	}
	(*cmd)[j] = '\0';
}

int		check_builtins(t_info *info, t_dlist *mcmd)
{
	char *cmd;
	int i;

	i = 0;
	cmd = 0;
	if (info->path)
		return (1);
	if (!mcmd)
		joincmd(&cmd, (t_dlist *)info->gbc->str);
	else
		joincmd(&cmd, (t_dlist *)mcmd->content);
	while (cmd[i] && cmd[i] != ' ')
		i++;
	if (!ft_strncmp(cmd, "echo", i))
	 	echo(cmd, 1);
	else if (!ft_strncmp(cmd, "cd", i))
		cd(cmd, info);
	else if (!ft_strncmp(cmd, "pwd", i))
		pwd(info, cmd, 1);
	else if (!ft_strncmp(cmd, "export", i))
		ft_export(cmd, info, 1);
	else if (!ft_strncmp(cmd, "unset", i))
	 	unset(cmd, info);
	else if (!ft_strncmp(cmd, "env", i))
	 	env(cmd, info, 1);
	else if (!ft_strncmp(cmd, "exit", i))
		ft_exit(cmd, info, 0);
	else
	{
		free(cmd);
		return (1);
	}
	free(cmd);
	return (0);
}

char	**dup_env(t_list *env)
{
	char **ret;
	int i;

	ret = malloc(sizeof(char *) * ft_lstsize(env));
	i = -1;
	if (!ret)
		return (0);
	while (env)
	{
		ret[++i] = ft_strdup((char *)env->content);
		env = env->next;
	}
	ret[i] = 0;
	return (ret);
}

int		check_exec(t_info *info, t_dlist *mcmd)
{
	char *tmp;
	pid_t pid;

	if (!mcmd)
		joincmd(&tmp, (t_dlist *)info->gbc->str);
	else
		joincmd(&tmp, (t_dlist *)mcmd);
	pid = fork();
	if (pid == -1)
		ft_exit(0, info, err_pid);
	else if (!pid)
	{
		if ((tmp[0] == '.' && tmp[1] == '/') || tmp[0] == '/')
			execve(tmp, ft_split(tmp, ' '), dup_env(info->env));
		else
			execve((char *)info->path->content,
			ft_split(tmp, ' '), dup_env(info->env));
	}
	else
		waitpid(pid, &errno, 0);
	errno = errno % 255;
	free(tmp);
	return (0);
}

void	exec_command(t_info *info)
{
	t_dlist	*tmp5;
	t_token	*token;

	if (!check_command(info))
	{
		// t_gbc *tmp;
		// int i = 0;
		// tmp = info->gbc;
		// while (tmp)
		// {
		// 	printf ("%d:\n", i);
		// 	if (tmp->type == STR)
		// 	{
		// 		printf("OUAIIIIII\n");
		// 		t_dlist *tmp2;
		// 		tmp2 = tmp->str;
		// 		printf("|%s|\n", (char *)tmp2->content);
		// 	}
		// 	else if (tmp->type == TOKEN)
		// 	{
		// 		t_dlist *tmp2;
		// 		tmp2 = tmp->str;
		// 		while (tmp2)
		// 		{
		// 			t_token *tmp3;
		// 			tmp3 = tmp2->content;
		// 			printf("|%s|\n", (char *)tmp3->value);
		// 			tmp2 = tmp2->next;
		// 		}
		// 	}
		// 	tmp = tmp->next;
		// 	i++;
		// }
		// printf("\npath:\n");
		// t_list *tmppth;
		// tmppth = info->path;
		// tmp = info->gbc;
		// while (tmppth)
		// {
		// 	printf("-> |%s|\n", (char *)tmppth->content);
		// 	tmppth = tmppth->next;
		// }
		// printf("\n");


			tmp5 = info->gbc->str;
			token = tmp5->content;
			if (info->gbc->next || (token->type >= 3 && token->type <= 6))
				redirection(info);
			else if (check_builtins(info, 0))
				check_exec(info, 0);
	}
	ft_lstclear(&info->path, &ft_memdel);

	gbcclear(&info->gbc);
}
