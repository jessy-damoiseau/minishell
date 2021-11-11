/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jessy <jessy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 00:58:37 by jessy             #+#    #+#             */
/*   Updated: 2021/10/29 18:12:34 by jessy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env2(char *cmd)
{
	int			i;
	struct stat	sb;

	i = 3;
	while (cmd[i] == ' ')
		i++;
	ft_putstr_fd("env: '", 2);
	ft_putstr_fd(&cmd[i], 2);
	if (!stat(&cmd[i], &sb))
	{
		ft_putstr_fd("’: Permission denied\n", 2);
		errno = 126;
	}
	else
	{
		ft_putstr_fd("’: No such file or directory\n", 2);
		errno = 127;
	}
}

void	env(char *cmd, int fd)
{
	t_list		*tmp;
	int			i;

	i = 3;
	tmp = g_info.env;
	while (cmd[i] == ' ')
		i++;
	if (ft_strcmp(cmd, "env") && cmd[i])
		env2(cmd);
	else
	{
		while (tmp)
		{
			ft_putstr_fd(tmp->content, fd);
			ft_putstr_fd("\n", fd);
			tmp = tmp->next;
		}
		errno = 0;
	}
}

void	unset2(char *cmd, int i, int j)
{
	t_list	*tmp;
	t_list	*tmp2;
	char	*tfchier;

	tmp = g_info.env;
	while (tmp->next)
	{
		tfchier = tmp->next->content;
		if (!ft_strncmp(tmp->next->content, &cmd[i], j)
			&& (tfchier[j] == '=' || !tfchier[j]))
		{
			tmp2 = tmp->next->next;
			free(tmp->next->content);
			free(tmp->next);
			tmp->next = tmp2;
			break ;
		}
		tmp = tmp->next;
	}
}

void	unset(char *cmd)
{
	int	i;
	int	j;

	i = 5;
	j = -1;
	while (cmd[i] == ' ')
		i++;
	while (cmd[i + ++j])
	{
		if (!ft_isalnum(cmd[i + j]) && cmd[i + j] != ' ' && cmd[i + j] != '_')
		{
			ft_putstr_fd("unset: '", 2);
			ft_putstr_fd(&cmd[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			errno = 1;
			return ;
		}
	}
	unset2(cmd, i, j);
	errno = 0;
}

void	echo(char *cmd, int fd)
{
	int	i;
	int	returnendline;

	i = 4;
	returnendline = 0;
	while (cmd[i] == ' ')
		i++;
	if (cmd[i] == '-' && cmd[i + 1] == 'n' && (!cmd[i + 2] || cmd[i + 2] == ' '))
	{
		if (!cmd[i + 2])
			i += 2;
		else
			i += 3;
		returnendline = 1;
	}
	while (cmd[i] && cmd[i] == ' ')
		i++;
	ft_putstr_fd(&cmd[i], fd);
	if (!returnendline)
		ft_putstr_fd("\n", fd);
	errno = 0;
}
