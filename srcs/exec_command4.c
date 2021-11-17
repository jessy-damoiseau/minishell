/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 17:40:27 by jessy             #+#    #+#             */
/*   Updated: 2021/11/17 13:14:47 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_command5(t_dlist	**dlsttmp1,
	t_token *token1, char *chrtmp1)
{
	t_dlist	*dlsttmp2;
	int		i;

	if (!check_exist(chrtmp1) && !check_binaire(chrtmp1))
		*dlsttmp1 = fill_gbc(*dlsttmp1, 0);
	else if (((chrtmp1[0] == '.' && chrtmp1[1] == '/')
			|| chrtmp1[0] == '/'))
		return (check_command6(chrtmp1, dlsttmp1));
	i = check_path(chrtmp1);
	if (i == 2)
	{
		clear_cmd_lst(dlsttmp1);
		return (1);
	}
	if (!i || (g_info.gbc && token1->type != 2))
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

int	check_command3(t_token *token1, t_dlist *dlsttmp1, t_dlist *list)
{
	char	*chrtmp1;

	dlsttmp1 = list;
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
				if (check_command5(&dlsttmp1, token1, chrtmp1))
					return (1);
		}
	}
	return (0);
}

void	check_not_only_void_before(t_token *token1, t_dlist *dlsttmp1, t_dlist **list)
{
	t_dlist	*tmp;
	t_dlist	*iter;
	// {
	// 		t_token	*tok;
	// 	t_dlist	*iter2 = dlsttmp1;
	// 	while (iter2)
	// 	{
	// 		tok = iter2->content;
	// 		iter2 = iter2->prev;
	// 		printf("|%s|%d|\n", (char *)tok->value, tok->type);
	// 	}	
	// }
	iter = dlsttmp1;
	if (iter->prev)
	{
		iter = iter->prev;
		token1 = iter->content;
		tmp = iter;
		while (iter)
		{
			token1 = iter->content;
			if (token1->type != 2)
				return ;
			if (!iter->prev)
				break ;
			iter = iter->prev;
		}
		// printf("|%s|\n", (char *)token1->value);
		// printf("%p\n", iter);
		// token1 = tmp->content;
		// printf("|%s|\n", (char *)token1->value);
		// printf("%p\n", tmp);
		clear_sublst(iter, tmp);
		// tmp = list;
		// token1 = tmp->content;
		*list = dlsttmp1;
	}
}

int	check_command2(t_token *token1, t_dlist *dlsttmp1, t_dlist *list)
{
	t_dlist	*tmp;

	tmp = dlsttmp1;
	// token1 = dlsttmp1->content;
	// printf("|%s|\n", (char *)token1->value);
	if (token1->type != 2)
		check_not_only_void_before(token1, dlsttmp1, &list);
	if (token1->type == 2)
	{
		while (token1->type == 2)
		{
			if (!dlsttmp1->prev)
			{
				clear_sublst(dlsttmp1, tmp);
				return (1);
			}
			else
				dlsttmp1 = dlsttmp1->prev;
			token1 = dlsttmp1->content;
		}
		if (token1->type >= 3 && token1->type <= 6)
		{
			ft_putstr_fd("bash: syntax error near ", 2);
			ft_putstr_fd("unexpected token `newline'\n", 2);
			clear_cmd_lst(&g_info.cmd);
			errno = 1;
			return (1);
		}
	}
	// token1 = dlsttmp1->content;
	// printf("|%s|\n", (char *)token1->value);
	return (check_command3(token1, dlsttmp1, list));
}

int	check_command(t_dlist *list)
{
	t_token	*token1;
	t_dlist	*dlsttmp1;

	dlsttmp1 = list;
	if (!dlsttmp1)
		return (1);
	while (dlsttmp1->next)
		dlsttmp1 = dlsttmp1->next;
	token1 = dlsttmp1->content;
	if (token1->type >= 3 && token1->type <= 6)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
		clear_cmd_lst(&g_info.cmd);
		errno = 1;
		return (1);
	}
	return (check_command2(token1, dlsttmp1, list));
}
