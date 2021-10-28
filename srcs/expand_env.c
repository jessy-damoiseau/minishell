/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jessy <jessy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 23:50:27 by pgueugno          #+#    #+#             */
/*   Updated: 2021/10/28 18:29:16 by jessy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	concat_node(t_token *atoken, t_token *btoken, t_dlist **iter)
{
	t_token	*token;
	char	*tmp;

	token = (*iter)->content;
	tmp = token->value;
	token->value = ft_strjoin(atoken->value, btoken->value);
	free(tmp);
}

void	concat_narrow_litvalue(void)
{
	t_dlist	*iter;
	t_dlist	*tmp;

	if (info.nbpipe)
		iter = info.cmdpipe->content;
	else
		iter = info.cmd;
	while (iter)
	{
		if (iter->next)
		{
			if (find_token_type(literal, iter->content)
				&& find_token_type(literal, iter->next->content))
			{
				tmp = iter;
				iter = iter->next;
				concat_node(tmp->content, iter->content, &iter);
				clear_cmd_node(&tmp);
			}
		}
		if (iter)
			iter = iter->next;
	}
}

int	is_env_literal(t_dlist **iter)
{
	t_token	*token;

	if (find_token_type(dollar, (*iter)->content))
	{
		if (!(*iter)->next)
			return (1);
		if ((*iter)->next)
		{
			token = (*iter)->next->content;
			if (token->type != literal && token->type != errno_call)
				token = (*iter)->content;
			token->type = literal;
		}
	}
	return (0);
}

void	expand_env(void)
{
	t_dlist	*iter;

	if (info.nbpipe)
		iter = info.cmdpipe->content;
	else
		iter = info.cmd;
	while (iter)
	{
		if (is_env_literal(&iter))
			return ;
		if (find_errno_type(iter))
			replace_errno(ft_itoa(errno), &iter);
		else if (find_token_type(dollar, iter->content))
			find_env_var(&iter);
		if (find_errno_type(iter))
			replace_errno(ft_itoa(errno), &iter);
		if (iter)
			iter = iter->next;
	}
	concat_narrow_litvalue();
	return ;
}
