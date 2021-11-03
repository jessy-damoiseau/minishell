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

void	replace_node_value(char *evar, t_dlist **iter)
{
	t_token	*token;

	token = (*iter)->content;
	ft_memdel(&token->value);
	while (*evar)
	{
		if (*evar == '=')
		{
			evar++;
			token->value = ft_strdup(evar);
		}
		if (*evar != '\0')
			evar++;
	}
}

int	check_if_value_in_env(char *envval, char *cmdval)
{
	size_t	e;
	size_t	c;

	e = ft_strlen_utils(envval, '='); // il faut prendre comme limite au parsing de la var expand les symboles +-=%#@!±§~[]{}/., (tout hors chiffres et lettre en gros) comme en dehors du champ d'en a parser
	c = ft_strlen(cmdval);
	if (!ft_strncmp(envval, cmdval, e) && e == c)
		return (1);
	// if (!ft_strncmp(envval, cmdval, e)) // plus laxiste, permet de le $a+$b sans le +
	// 	return (1);
	return (0);
}

int	find_env_var(t_dlist **iter)
{
	t_dlist	*tmp;
	t_list	*env;
	t_token	*token;

	tmp = *iter;
	*iter = (*iter)->next;
	token = (*iter)->content;
	if (g_info.env)
	{
		env = g_info.env;
		while (env)
		{
			if (check_if_value_in_env(env->content, token->value))
			{
				replace_node_value(env->content, iter);
				clear_cmd_node(&tmp);
				return (1);
			}
			env = env->next;
		}
	}
	clear_cmd_node(&tmp);
	clear_cmd_node(iter);
	return (0);
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
