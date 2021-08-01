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

t_dlist	*fill_gbc(t_dlist *lst, t_info *info)
{
	t_token *token;
	t_dlist *tmp;
	t_dlist *forgbc;
	int i;
	int check;

	token = lst->content;
	tmp = lst;
	i = 0;
	check = 0;
	while (tmp && (!(token->type >= 3 && token->type <= 6)))
	{
		tmp = tmp->next;
		if (tmp)
			token = tmp->content;
		i++;
		check = 1;
	}


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
	if (lst)
	{
		if (lst->next)
			tmp = lst->next;
		lst->next = 0;
		lstaddback_gbc(&info->gbc, newgbc(STR, -1, lst->content));
		return (tmp);
	}
	return (0);
}

int check_path(char *tmp1, t_info *info)
{
	char	*env_path;
	char	**path;
	char	*checkpth;
	struct stat sb;
	int		i;

	i = 0;
	env_path = getenv("PATH");
	path = ft_split(env_path, ':');
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

int check_command(t_info *info)
{
	
}

int	check_command(t_info *info)
{
	t_token	*token;
	t_dlist	*tmp;
	t_dlist	*tmp2;
	char	*tmp1;

	tmp = info->cmd;
	while (tmp->next)
		tmp = tmp->next;
	token = tmp->content;
	if (token->type == 0
	|| (token->type >= 3 && token->type <= 6))
	{
		errno = 1;
		return (1);
	}
	tmp = info->cmd;
	while (tmp)
	{
		token = tmp->content;
		tmp1 = token->value;
		//printf ("VALUE : |%s|\n", tmp1);
		if (!ft_strcmp(tmp1, "echo")
		|| !ft_strcmp(tmp1, "cd")
		|| !ft_strcmp(tmp1, "pwd")
		|| !ft_strcmp(tmp1, "export")
		|| !ft_strcmp(tmp1, "unset")
		|| !ft_strcmp(tmp1, "env")
		|| !ft_strcmp(tmp1, "exit")
		|| !ft_strncmp(tmp1, "./", 2)
		|| !check_path(tmp1, info)
		|| (info->gbc && token->type != 2))
			tmp = fill_gbc(tmp, info);
		else if (!ft_strcmp(tmp1, " "))
		{
			//printf("check\n");
			tmp2 = tmp;
			tmp = tmp->next;
			free(tmp2);
		}
		else
		{
			ft_putstr_fd(tmp1, 2);
			ft_putstr_fd(": command not found\n", 2);
			errno = 127;
			return (1);
		}
	}
	printf("checkdone\n");
	info->cmd = 0;
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

int	check_builtins(t_info *info)
{
	char *cmd;
	int i;

	i = 0;
	cmd = 0;
	if (info->path)
		return (1);
	joincmd(&cmd, (t_dlist *)info->gbc->str);
	printf("cmd: |%s|\n", cmd);
	printf("return cmd: -> \n");
	while (cmd[i] && cmd[i] != ' ')
		i++;
	if (!ft_strncmp(cmd, "echo", i))
	 	echo(cmd, 1);
	else if (!ft_strncmp(cmd, "cd", i))
		cd(cmd);
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
int check_exec(t_info *info)
{
	char *tmp;
	char **cmd;
	pid_t pid;

	cmd = 0;
	printf("return cmd: -> \n");
	joincmd(&tmp, (t_dlist *)info->gbc->str);
	if (!info->path && tmp[0] != '.' && tmp[1] != '/')
	{
		free(tmp);
		return (1);
	}
	cmd = ft_split(tmp, ' ');
	if (tmp[0] == '.' && tmp[1] == '/')
	{
		pid = fork();
		if (!pid)
			execve(tmp, cmd, info->evrm);
		else
			wait(0);
	}
	else
	{
		pid = fork();
		if (!pid)
			execve((char *)info->path->content, cmd, info->evrm);
		else
			wait(0);
	}
	free(cmd);
	return (0);
}

void	exec_command(t_info *info)
{
	t_gbc *tmp;
	int i = 0;
	if (!check_command(info))
	{
		tmp = info->gbc;
		while (tmp)
		{
			printf ("%d:\n", i);
			if (tmp->type == STR)
			{
				printf("OUAIIIIII\n");
				t_dlist *tmp2;
				tmp2 = tmp->str;
				printf("|%s|\n", (char *)tmp2->content);
			}
			else if (tmp->type == TOKEN)
			{
				printf("NYYYYYAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
				t_dlist *tmp2;
				tmp2 = tmp->str;
				while (tmp2)
				{
					t_token *tmp3;
					tmp3 = tmp2->content;
					printf("|%s|\n", (char *)tmp3->value);
					tmp2 = tmp2->next;
				}
			}
			tmp = tmp->next;
			i++;
		}
		printf("\npath:\n");
		t_list *tmppth;
		tmppth = info->path;
		tmp = info->gbc;
		while (tmppth)
		{
			printf("-> |%s|\n", (char *)tmppth->content);
			tmppth = tmppth->next;
		}
		printf("\n");
			if (info->gbc->next)
				redirection(info);
			else if (check_builtins(info))
			{
				if (check_exec(info))
				{
					printf("mdr faut check autre chose ah ah AHHH AHHH AHHH\n");
					//check_other();
				}
			}
	}
	ft_lstclear(&info->path, &ft_memdel);
	gbcclear(&info->gbc);
}
