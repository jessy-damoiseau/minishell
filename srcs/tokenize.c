/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jessy <jessy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 23:50:04 by pgueugno          #+#    #+#             */
/*   Updated: 2021/10/28 18:28:40 by jessy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token		g_tab_token[] =
{
	{"|", pipeline},
	{"$", dollar},
	{" ", space},
	{">", redir_right},
	{"<", redir_left},
	{">>", dble_redir_left},
	{"<<", dble_redir_right},
	{"\'", sgle_quote},
	{"\"", dble_quote},
	{"?", errno_call},
	{0, 0},
};

t_token_type	ft_define_token(char charset)
{
	int				i;
	t_token_type	type;
	char			*str;

	i = 0;
	type = literal;
	while (g_tab_token[i].value)
	{
		str = g_tab_token[i].value;
		if (str[0] == charset)
		{
			type = g_tab_token[i].type;
			return (type);
		}
		i++;
	}
	return (type);
}

t_token	*ft_malloc_token(char *input, t_token_type type, int i, int j)
{
	t_token	*token;
	char	*s;

	s = NULL;
	token = malloc(sizeof(t_token));
	if (!token)
		return (0);
	token->type = type;
	if (!j)
	{
		if (ft_go_malloc(&s, sizeof(char) * 2))
			return (0);
		ft_strncpy(s, input, 1);
	}
	else
	{
		if (ft_go_malloc(&s, sizeof(char) * (j - i + 2)))
			return (0);
		ft_strncpy(s, input, j - i + 1);
	}
	token->value = ft_strdup(s);
	free(s);
	return (token);
}

int	is_it_double_redir(char *s, int i)
{
	if ((unsigned int)i < strlen(s))
	{
		if (s[i] == s[i + 1])
		{
			if (ft_define_token(s[i]) == redir_left)
				return (dble_redir_left);
			if (ft_define_token(s[i]) == redir_right)
				return (dble_redir_right);
		}
	}
	return (0);
}

t_token	*ft_find_token(char *s, int *i)
{
	t_token	*token;
	int		j;

	if (literal != ft_define_token(s[*i]))
	{
		j = *i;
		if (is_it_double_redir(s, *i))
		{
			token = ft_malloc_token(s + *i,
					is_it_double_redir(s, *i), *i, j + 1);
			(*i)++;
		}
		else
			token = ft_malloc_token(s + *i, ft_define_token(s[*i]), *i, 0);
	}
	else
	{
		j = *i;
		while (s[j] && literal == ft_define_token(s[j]))
			j++;
		token = ft_malloc_token(s + *i, ft_define_token(s[*i]), *i, j - 1);
		*i = j - 1;
	}
	return (token);
}

void	ft_create_token(char *s)
{
	int		i;
	t_token	*token;
	t_dlist	*new;

	i = 0;
	info.cmd = NULL;
	while (s[i])
	{
		token = ft_find_token(s, &i);
		if (!token)
			ft_exit(0, err_malloc);
		new = dlstnew(token);
		dlstadd_back(&info.cmd, new);
		i++;
	}
	parse_token();
	return ;
}
