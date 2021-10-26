/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 23:50:23 by pgueugno          #+#    #+#             */
/*   Updated: 2021/10/26 23:50:24 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	delete_dble_quotes(t_dlist **iter, t_dlist *node, t_info *info)
{
	int		flag;
	t_dlist	*tmp;
	t_token	*token;

	tmp = *iter;
	flag = 1;
	if ((*iter)->next)
		*iter = (*iter)->next;
	while (*iter)
	{
		token = (*iter)->content;
		if (*iter == node)
			flag = 0;
		if (token->type == dble_quote && !flag)
		{
			clear_cmd_node(iter, info);
			clear_cmd_node(&tmp, info);
			return (1);
		}
		else if (token->type == dble_quote && flag)
			return (0);
		*iter = (*iter)->next;
	}
	return (0);
}

static void	find_quotes(t_info *info, t_dlist *node)
{
	t_dlist	*iter;

	iter = info->cmd;
	while (iter && iter != node)
	{
		if (find_token_type(dble_quote, iter->content))
			delete_dble_quotes(&iter, node, info);
		if (iter)
			iter = iter->next;
	}
}

void	parse_env(t_info *info)
{
	t_dlist	*iter;

	iter = info->cmd;
	while (iter)
	{
		if (find_token_type(dollar, iter->content))
			find_quotes(info, iter);
		iter = iter->next;
	}
}
