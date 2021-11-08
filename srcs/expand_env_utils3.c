/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_utils3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 13:24:40 by pgueugno          #+#    #+#             */
/*   Updated: 2021/11/08 13:38:44 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_there_a_space(char *str)
{
	while (*str)
	{
		if (*str == ' ')
			return (1);
		str++;
	}
	return (0);
}

static void	add_new_lit_node(t_dlist **tmp, char *str, int *i)
{
	int		j;
	char	*s;
	t_dlist	*new;

	j = *i;
	while (str[j] != ' ' && str[j] != '\0')
		j++;
	s = ft_strndup(str + *i, j);
	new = dlstnew(ft_malloc_token(s, literal, 0, ft_strlen(s)));
	dlstinsert_node(&g_info.cmd, *tmp, new);
	*tmp = (*tmp)->next;
	*i = j;
	free(s);
}

static void	retokenize_rest(t_dlist **iter, char *str, int i)
{
	t_dlist	*tmp;
	t_dlist	*new;

	tmp = *iter;
	tmp = tmp->next;
	if (str[i + 1] != '\0')
		i++;
	while (str[i])
	{
		if (str[i] == ' ')
		{
			new = dlstnew(ft_malloc_token(" ", space, 0, 1));
			dlstinsert_node(&g_info.cmd, tmp, new);
			tmp = tmp->next;
		}
		else if (str[i] != i)
			add_new_lit_node(&tmp, str, &i);
		if (str[i] != '\0')
			i++;
	}
}

void	reparse_expanded_value(t_dlist **iter)
{
	char	*str;
	t_token	*token;
	t_dlist	*new;
	int		i;

	i = 0;
	token = (*iter)->content;
	str = token->value;
	if (is_there_a_space(token->value))
	{
		while (str[i])
		{
			if (str[i] == ' ')
				break ;
			i++;
		}
		token->value = ft_strndup(str, i);
		new = dlstnew(ft_malloc_token(" ", space, 0, 1));
		// if (g_info.nbpipe)
		// 	dlstinsert_node(&g_info.cmdpipe, *iter, new); // cas des pipes à prévoir
		dlstinsert_node(&g_info.cmd, *iter, new);
		retokenize_rest(iter, str, i);
		free(str);
	}
}
