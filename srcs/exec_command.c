/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jessy <jessy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 01:09:41 by jessy             #+#    #+#             */
/*   Updated: 2021/10/28 20:22:54 by jessy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_end(t_dlist **alst)
{
	t_token	*token;
	t_dlist	*tmp1;
	t_dlist	*tmp2;

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

void	fill_gbc2(int *i, int *check, t_dlist *tmp)
{
	t_token	*token;

	token = tmp->content;
	while (tmp && (!(token->type >= 3 && token->type <= 6)))
	{
		tmp = tmp->next;
		if (tmp)
			token = tmp->content;
		(*i)++;
		*check = 1;
	}
}

t_dlist	*fill_gbc(t_dlist *lst, int i)
{
	t_dlist	*tmp;
	t_dlist	*forgbc;
	int		check;

	tmp = lst;
	check = 0;
	if (!i)
		fill_gbc2(&i, &check, tmp);
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
		lstaddback_gbc(&info.gbc, newgbc(TOKEN, -1, (void *)forgbc));
	}
	return (lst);
}

int	check_path2(char **path, char *tmp1)
{
	struct stat	sb;
	int			i;
	char		*checkpth;

	i = -1;
	while (path[++i])
	{
		checkpth = strpthjoin(path[i], tmp1);
		if (!stat(checkpth, &sb))
		{
			ft_lstadd_back(&info.path, ft_lstnew((void *)checkpth));
			return (0);
		}
		free(checkpth);
	}
	return (1);
}

int	check_path(char *tmp1)
{
	char		*env_path;
	char		**path;
	t_list		*tmp;

	tmp = info.env;
	while (tmp && ft_strncmp(tmp->content, "PATH=", 5))
		tmp = tmp->next;
	env_path = tmp->content;
	path = ft_split(&env_path[5], ':');
	if (!check_path2(path, tmp1))
	{
		free_dbl(path);
		return (0);
	}
	free_dbl(path);
	return (1);
}

int	check_exist(char *str)
{
	struct stat	sb;

	if (stat(str, &sb))
		return (1);
	return (0);
}

int	check_binaire(char *str)
{
	int		fd;
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

void	check_command4(t_dlist **dlsttmp1, int i)
{
	t_token	*token2;
	t_dlist	*dlsttmp2;

	token2 = (*dlsttmp1)->next->content;
	dlsttmp2 = (*dlsttmp1)->next;
	while (dlsttmp2 && token2->type != 9)
	{
		dlsttmp2 = dlsttmp2->next;
		if (dlsttmp2)
		{
			token2 = dlsttmp2->content;
			i++;
		}
	}
	dlsttmp2 = dlsttmp2->next;
	if (dlsttmp2)
		token2 = dlsttmp2->content;
	while (dlsttmp2 && token2->type == space)
	{
		i++;
		dlsttmp2 = dlsttmp2->next;
		token2 = dlsttmp2->content;
	}
	(*dlsttmp1) = fill_gbc((*dlsttmp1), i);
}

int	check_command6(char *chrtmp1)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(chrtmp1, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	clear_cmd_lst(&info.cmd);
	errno = 127;
	return (1);
}

int	check_command7(char	*chrtmp1)
{
	ft_putstr_fd(chrtmp1, 2);
	ft_putstr_fd(": command not found\n", 2);
	clear_cmd_lst(&info.cmd);
	errno = 127;
	return (1);
}

int	check_command5(t_dlist	**dlsttmp1,
	t_token *token1, char *chrtmp1)
{
	t_dlist	*dlsttmp2;

	if (!check_exist(chrtmp1) && !check_binaire(chrtmp1))
		*dlsttmp1 = fill_gbc(*dlsttmp1, 0);
	else if (((chrtmp1[0] == '.' && chrtmp1[1] == '/')
			|| chrtmp1[0] == '/'))
		return (check_command6(chrtmp1));
	else if (!check_path(chrtmp1)
		|| (info.gbc && token1->type != 2))
			*dlsttmp1 = fill_gbc(*dlsttmp1, 0);
	else if (!ft_strcmp(chrtmp1, " "))
	{
		dlsttmp2 = *dlsttmp1;
		*dlsttmp1 = (*dlsttmp1)->next;
		free(dlsttmp2);
	}
	else
		return (check_command7(chrtmp1));
	return (0);
}

int	check_command8(char *chrtmp1)
{
	if (!ft_strcmp(chrtmp1, "echo")
		|| !ft_strcmp(chrtmp1, "cd")
		|| !ft_strcmp(chrtmp1, "pwd")
		|| !ft_strcmp(chrtmp1, "export")
		|| !ft_strcmp(chrtmp1, "unset")
		|| !ft_strcmp(chrtmp1, "env")
		|| !ft_strcmp(chrtmp1, "exit"))
		return (1);
	return (0);
}

int	check_command3(t_token *token1, t_dlist *dlsttmp1)
{
	char	*chrtmp1;

	if (info.nbpipe)
		dlsttmp1 = info.cmdpipe->content;
	else
		dlsttmp1 = info.cmd;
	while (dlsttmp1)
	{
		token1 = dlsttmp1->content;
		if (token1->type >= 3 && token1->type <= 6)
			check_command4(&dlsttmp1, 2);
		else
		{
			chrtmp1 = token1->value;
			if (check_command8(chrtmp1))
				dlsttmp1 = fill_gbc(dlsttmp1, 0);
			else
			{
				if (check_command5(&dlsttmp1, token1, chrtmp1))
				return (1);
			}
		}
	}
	return (0);
}

int	check_command2(t_token *token1, t_dlist *dlsttmp1)
{
	if (token1->type == 2)
	{
		while (token1->type == 2)
		{
			dlsttmp1 = dlsttmp1->prev;
			token1 = dlsttmp1->content;
		}
		if (token1->type >= 3 && token1->type <= 6)
		{
			ft_putstr_fd("bash: syntax error near ", 2);
			ft_putstr_fd("unexpected token `newline'\n", 2);
			clear_cmd_lst(&info.cmd);
			errno = 1;
			return (1);
		}
	}
	return (check_command3(token1, dlsttmp1));
}

int	check_command(void)
{
	t_token	*token1;
	t_dlist	*dlsttmp1;

	if (info.nbpipe)
		dlsttmp1 = info.cmdpipe->content;
	else
		dlsttmp1 = info.cmd;
	if (!dlsttmp1)
		return (1);
	while (dlsttmp1->next)
		dlsttmp1 = dlsttmp1->next;
	token1 = dlsttmp1->content;
	if (token1->type >= 3 && token1->type <= 6)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
		clear_cmd_lst(&info.cmd);
		errno = 1;
		return (1);
	}
	return (check_command2(token1, dlsttmp1));
}

void	joincmd2(t_dlist *tmp1, int *i)
{
	t_token	*tmp;

	while (tmp1)
	{
		tmp = tmp1->content;
		*i += ft_strlen((char *)tmp->value);
		tmp1 = tmp1->next;
	}
}

void	joincmd(char **cmd, t_dlist *lst)
{
	t_dlist	*tmp1;
	t_token	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	joincmd2(lst, &i);
	if (ft_go_malloc(cmd, (sizeof(char) * i + 1)))
		return ;
	tmp1 = lst;
	while (tmp1)
	{
		i = 0;
		tmp = tmp1->content;
		while (((char *)tmp->value)[i])
			(*cmd)[j++] = ((char *)tmp->value)[i++];
		tmp1 = tmp1->next;
	}
	(*cmd)[j] = '\0';
}

int	check_builtins2(char *cmd, int i)
{
	if (!ft_strncmp(cmd, "export", i))
		ft_export(cmd, 1);
	else if (!ft_strncmp(cmd, "unset", i))
		unset(cmd);
	else if (!ft_strncmp(cmd, "env", i))
		env(cmd, 1);
	else if (!ft_strncmp(cmd, "exit", i))
		ft_exit(cmd, 0);
	else
	{
		free(cmd);
		return (1);
	}
	free(cmd);
	return (0);
}

int	check_builtins(t_dlist *mcmd)
{
	char	*cmd;
	int		i;

	i = 0;
	cmd = 0;
	if (info.path)
		return (1);
	if (!mcmd)
		joincmd(&cmd, (t_dlist *)info.gbc->str);
	else
		joincmd(&cmd, (t_dlist *)mcmd->content);
	while (cmd[i] && cmd[i] != ' ')
		i++;
	if (!ft_strncmp(cmd, "echo", i))
		echo(cmd, 1);
	else if (!ft_strncmp(cmd, "cd", i))
		cd(cmd);
	else if (!ft_strncmp(cmd, "pwd", i))
		pwd(cmd, 1);
	else
		return (check_builtins2(cmd, i));
	free(cmd);
	return (0);
}

char	**dup_env(t_list *env)
{
	char	**ret;
	int		i;

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

int	check_exec(t_dlist *mcmd)
{
	char	*tmp;
	char	**str;
	pid_t	pid;

	if (!mcmd)
		joincmd(&tmp, (t_dlist *)info.gbc->str);
	else
		joincmd(&tmp, (t_dlist *)mcmd);
	pid = fork();
	str = ft_split(tmp, ' ');
	if (pid == -1)
		ft_exit(0, err_pid);
	else if (!pid)
	{
		if ((tmp[0] == '.' && tmp[1] == '/') || tmp[0] == '/')
			execve(str[0], str, dup_env(info.env));
		else
			execve((char *)info.path->content,
				str, dup_env(info.env));
	}
	else
		waitpid(pid, &errno, 0);
	free_dbl(str);
	errno = errno % 255;
	free(tmp);
	return (0);
}

void	exec_command(void)
{
	t_dlist	*tmp5;
	t_token	*token;

	if (!check_command())
	{
		t_gbc *tmp;
		int i = 0;
		tmp = info.gbc;
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
		tmppth = info.path;
		tmp = info.gbc;
		while (tmppth)
		{
			printf("-> |%s|\n", (char *)tmppth->content);
			tmppth = tmppth->next;
		}
		printf("\n");







		
		tmp5 = info.gbc->str;
		token = tmp5->content;
		if (info.gbc->next || (token->type >= 3 && token->type <= 6))
			redirection();
		else if (check_builtins(0))
			check_exec(0);
	}
	ft_lstclear(&info.path, &ft_memdel);
	gbcclear(&info.gbc);
}
