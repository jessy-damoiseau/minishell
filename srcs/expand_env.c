/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 23:50:27 by pgueugno          #+#    #+#             */
/*   Updated: 2021/11/12 00:43:10 by pgueugno         ###   ########.fr       */
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

int	find_narrow_litvalue(t_dlist *iter)
{
	if (iter->prev)
		if (find_token_type(literal, iter->content)
			&& find_token_type(literal, iter->prev->content))
			return (1);
	if (iter->next)
		if (find_token_type(literal, iter->content)
			&& find_token_type(literal, iter->next->content))
			return (1);
	return (0);
}

void	link_begin_and_end(t_dlist **iter, t_dlist *begin, t_dlist *end)
{
	if (!end)
	{
		clear_sublst((*iter)->next, end);
		begin->next = NULL;
	}
	else
	{
		clear_sublst((*iter)->next, end->prev);
		begin->next = end;
		end->prev = begin;
	}
}

void	concat_all(t_dlist **iter, t_dlist *begin, t_dlist *end, t_dlist *tmp)
{
	char	*tmp2;
	char	*tmp3;
	t_token	*tok;

	tok = tmp->content;
	tmp2 = tok->value;
	tmp = tmp->next;
	while (tmp != end)
	{
		tok = tmp->content;
		tmp3 = ft_strjoin(tmp2, tok->value);
		free(tmp2);
		tmp2 = tmp3;
		tmp = tmp->next;
	}
	tok = begin->content;
	tok->value = tmp3;
	link_begin_and_end(iter, begin, end);
}

void	find_close_litvalue(t_dlist	**iter)
{
	t_dlist	*begin;
	t_dlist	*end;
	int		i;
	t_dlist	*tmp;

	i = 0;
	begin = *iter;
	end = *iter;
	while (end)
	{
		if (!find_token_type(literal, end->content))
			break ;
		end = end->next;
	}
	tmp = begin;
	while (tmp != end)
	{
		i++;
		tmp = tmp->next;
	}
	if (i <= 1)
		return ;
	tmp = begin;
	concat_all(iter, begin, end, tmp);
}

void	concat_narrow_litvalue(void)
{
	t_dlist	*iter;

	if (g_info.nbpipe)
		iter = g_info.cmdpipe->content;
	else
		iter = g_info.cmd;
	find_close_litvalue(&iter);
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

	if (g_info.nbpipe)
		iter = g_info.cmdpipe->content;
	else
		iter = g_info.cmd;
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
			if (find_token_type(dollar, iter->content))
				find_env_var(&iter);
		if (iter)
			iter = iter->next;
	}
	concat_narrow_litvalue();
	return ;
}
