#include "minishell.h"

t_token		tab_token[] = 
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
	{0, 0},
};

t_token_type	ft_define_token(char charset)
{
	int				i;
	t_token_type	type;
	char			*str;

	i = 0;
	type = literal;
	while(tab_token[i].value)
	{
		str = tab_token[i].value;
		if (str[0] == charset)
		{
			type = tab_token[i].type;
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
		if (ft_go_malloc(&s, sizeof(char) * (j - i + 1)))
			return (0);
		ft_strncpy(s, input, j - i + 1);
	}
	token->value = ft_strdup(s);
	free(s);
	return (token);
}

t_token	*ft_find_token(char *s, int *i)
{
	t_token *token;
	int j;

	if (literal != ft_define_token(s[*i]))
		token = ft_malloc_token(s + *i, ft_define_token(s[*i]), *i, 0);
	else
	{
		j = *i;
		while(s[j] && literal == ft_define_token(s[j]))
			j++;
		token = ft_malloc_token(s + *i, ft_define_token(s[*i]), *i, j - 1);
		*i = j - 1;
	}
	return (token);
}

void	ft_create_token(char *s, t_info *info)
{
	int		i;
	t_token	*token;
	t_list	*new;

	i = 0;
	info->cmd = NULL;
	while(s[i])
	{
		token = ft_find_token(s, &i);
		if (!token)
			ft_exit(info, err_malloc);
		new = ft_lstnew(token);
		ft_lstadd_back(&info->cmd, new);
		i++;
	}
	// @Jessy si tu veux tester l'output
	// t_list *test;
	// test = info->cmd;
	// t_token *testtok;
	// while (test)
	// {
	// 	testtok = test->content;
	// 	printf("check token.value %s\n", (char *)testtok->value);
	// 	test = test->next;
	// }
	// ft_exit(info, no_err);
	return ;
}