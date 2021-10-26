#include "minishell.h"

int	find_token_type(t_token_type type, t_token *token)
{
	if (token->type == type)
		return (1);
	return (0);
}

void	clear_sublst(t_dlist *begin, t_dlist *end)
{
	t_dlist	*iter;
	t_dlist	*tmp;

	iter = begin;
	while (iter)
	{
		if (iter == end)
			break ;
		tmp = iter;
		iter = iter->next;
		clear_token(tmp, tmp->content);
	}
	clear_token(end, end->content);
}

t_dlist	*samequote_begin_or_end(t_dlist *begin, t_dlist *end, t_info *info)
{
	t_dlist	*new;

	new = end->next;
	if (!begin->prev)
	{
		clear_token(begin, begin->content);
		clear_token(end, end->content);
		info->cmd = NULL;
		return (NULL);
	}
	else
	{
		new = begin->prev;
		new->next = NULL;
		clear_token(begin, begin->content);
		clear_token(end, end->content);
		return (new);
	}
}

t_dlist	*clean_sublst(t_dlist *begin, t_dlist *end, t_info *info)
{
	t_dlist	*new;

	new = end->next;
	if (begin->next == end && !end->next)
		return (samequote_begin_or_end(begin, end, info));
	if (!begin->prev)
	{
		clear_sublst(begin, end);
		info->cmd = new;
		new->prev = NULL;
		return (new);
	}
	new->prev = begin->prev;
	begin->prev->next = new;
	clear_sublst(begin, end);
	return (new);
}

void	clear_buffer(char *buf, char *tmp, t_info *info)
{
	ft_memdel((void *)&tmp);
	ft_memdel((void *)&buf);
	ft_exit(0, info, err_malloc);
}
