/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jessy <jessy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 23:21:52 by jessy             #+#    #+#             */
/*   Updated: 2021/10/28 18:29:44 by jessy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_code(t_err_code err_code)
{
	if (err_code == 1)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("Error : Malloc failed\n", 2);
		return (1);
	}
	if (err_code == 2)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("Error : fork failed\n", 2);
		return (1);
	}
	if (err_code == 3)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("Error : open failed\n", 2);
		return (1);
	}
	return (0);
}

int	exit2bis(char *cmd, int *i, int j)
{
	int	nbspace;

	nbspace = 0;
	if (cmd[*i] == '-')
		j++;
	while (cmd[*i + ++j])
	{
		if (cmd[*i + j] == ' ' && cmd[*i + j + 1] != ' ' && cmd[*i + j + 1])
		{
			nbspace++;
			while (cmd[*i + j] == ' ')
				j++;
		}
	}
	return (nbspace);
}

int	exit2(char *cmd, int *i, int j)
{
	while (cmd[*i] && cmd[*i] == ' ')
		(*i)++;
	if (!cmd[*i])
	{
		clear_cmd_lst(&info.cmd);
		ft_lstclear(&info.env, &ft_memdel);
		if (!info.child)
			ft_putstr_fd("exit\n", 1);
		free(cmd);
		exit(errno);
	}
	return (exit2bis(cmd, i, j));
}

int	exit3(char *cmd, int i)
{
	int	ret;
	int	error;

	error = 0;
	ret = ft_atoll(&cmd[i], &error) % 256;
	if (!info.child)
		ft_putstr_fd("exit\n", 1);
	if (error)
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(&cmd[i], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		ret = 2;
	}
	return (ret);
}

void	ft_exit(char *cmd, t_err_code err_code)
{
	int	i;
	int	ret;

	i = 4;
	ret = errno;
	if (cmd)
	{
		if (exit2(cmd, &i, -1))
		{
			errno = 1;
			ft_putstr_fd("exit: too many arguments\n", 2);
			return ;
		}
		ret = exit3(cmd, i);
		free(cmd);
	}
	if (error_code(err_code))
		ret = 1;
	ft_lstclear(&info.env, &ft_memdel);
	exit(ret);
}
