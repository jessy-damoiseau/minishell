/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jessy <jessy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 00:57:23 by jessy             #+#    #+#             */
/*   Updated: 2021/10/28 00:58:10 by jessy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(t_info *info, char *cmd, int fd)
{
	if (!ft_strcmp(cmd, "pwd"))
	{
		ft_putstr_fd(info->pwd, fd);
		write(fd, "\n", 1);
		errno = 0;
	}
	else
	{
		ft_putstr_fd("pwd: too many arguments\n", 2);
		errno = 1;
	}
}

void	oldpwd(t_info *info)
{
	char	*str;
	t_list	*tmp;

	tmp = info->env;
	str = info->pwd;
	while (tmp && ft_strncmp(tmp->content, "OLDPWD=", 7))
		tmp = tmp->next;
	if (!tmp)
		ft_lstadd_back(&info->env, ft_lstnew(ft_strjoin("OLDPWD=", str)));
	else
	{
		free(tmp->content);
		tmp->content = ft_strjoin("OLDPWD=", str);
	}
}

int	cd1(char *cmd, int i, int *j)
{
	while (cmd[++i])
	{
		if (cmd[i] == ' ')
		{
			*j = i;
			while (cmd[*j] == ' ')
				(*j)++;
			if (cmd[*j] == '\0')
				break ;
			errno = 1;
			ft_putstr_fd("cd: too many argument\n", 2);
			return (1);
		}
	}
	return (0);
}

void	cd2(char *cmd, int j, char *str)
{
	if (!ft_strcmp(&cmd[3], "~") || j != 0)
	{
		chdir(str);
		errno = 0;
	}
	else
	{
		if (chdir(&cmd[3]) == -1)
		{
			ft_putstr_fd("cd: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			errno = 1;
		}
	}
}

void	cd(char *cmd, t_info *info)
{
	int		i;
	int		j;
	char	*str;

	i = 2;
	j = 0;
	oldpwd(info);
	str = getenv("HOME");
	if (cmd[2] != '\0')
	{
		if (cd1(cmd, i, &j))
			return ;
		cd2(cmd, j, str);
	}
	else
	{
		chdir(str);
		errno = 0;
	}
}
