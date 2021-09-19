#include "minishell.h"

void 	clear_token(t_dlist *list, t_token *token)
{
	ft_memdel(&token->value);
	ft_memdel(&list->content);
	ft_memdel((void *)&list);
}

void	clear_cmd_lst(t_dlist **lst)
{
	t_dlist	*tmp;
	t_token	*token;

	tmp = NULL;
	token = NULL;
	if (*lst)
	{
		while (*lst)
		{
			tmp = *lst;
			token = tmp->content;
			*lst = (*lst)->next;
			clear_token(tmp, token);
		}
	}
	*lst = NULL;
}

void	clear_cmd_node(t_dlist **lst, t_info *info)
{
	t_dlist *tmp;

	tmp = *lst;
	if (!(*lst)->prev)
	{
		*lst = (*lst)->next;
		if (*lst) // exception pour cas ou prev et next == NULL
			(*lst)->prev = NULL;
		info->cmd = *lst;
	}
	else if (!(*lst)->next)
	{
		*lst = (*lst)->prev;
		(*lst)->next = NULL;
	}
	else if ((*lst)->prev && (*lst)->next)
	{
		*lst = (*lst)->next;
		(*lst)->prev = tmp->prev;
		tmp->prev->next = *lst;
	}
	clear_token(tmp, tmp->content);
}

void	free_dbl(char **str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	while(i--)
		free(str[i]);
	free(str);
	str = 0;
}

// cette fonction peut etre amelioree avec un tab msg erreur vs enum code
// et une fonction qui fait la correspondance comme ft_define_token