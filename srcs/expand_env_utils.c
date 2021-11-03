/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 23:50:30 by pgueugno          #+#    #+#             */
/*   Updated: 2021/11/02 23:42:02 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_node_value(char **evar, t_dlist **iter)
{
	t_token	*token;
	char	*tmp;

	tmp = *evar;
	token = (*iter)->content;
	ft_memdel(&token->value);
	while (*tmp)
	{
		if (*tmp == '=')
		{
			tmp++;
			token->value = ft_strdup(tmp);
		}
		if (*tmp != '\0')
			tmp++;
	}
	free(*evar);
}

int	find_errno_type(t_dlist *lst)
{
	t_token	*token;

	if (lst)
	{
		token = lst->content;
		if (token->type == dollar && lst->next)
		{
			lst = lst->next;
			token = lst->content;
			if (!strcmp((char *)token->value, "?"))
				return (1);
		}
	}
	return (0);
}

void	replace_errno(char *str, t_dlist **lst)
{
	t_dlist	*tmp;
	t_token	*token;

	tmp = (*lst)->next;
	(*lst)->next = tmp->next;
	clear_cmd_node(&tmp);
	token = (*lst)->content;
	token->type = literal;
	free(token->value);
	token->value = ft_strdup(str);
	free(str);
}
