#include "minishell.h"

int    find_token_type(t_token_type type, t_token *token)
{
	if (token->type == type)
		return (1);
	return (0);
}

void	clear_sublst(t_dlist *begin, t_dlist *end)
{
	t_dlist *iter;
	t_dlist *tmp;

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
	t_dlist *new;

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

char	*concate_node(t_dlist **sub, t_token_type type, t_info *info)
{
	t_token	*token;
	char	*tmp;
	char	*buf;

	buf = ft_strdup("");
	tmp = NULL;
	if (!buf)
		clear_buffer(buf, tmp, info);
	while (*sub)
	{
		if (find_token_type(type, (*sub)->content))
			break ;
		token = (*sub)->content;
		tmp = buf;
		buf = ft_strjoin(buf, token->value);
		if (!buf)
			clear_buffer(buf, tmp, info);
		ft_memdel((void *)&tmp);
		*sub = (*sub)->next;
	}
	return (buf);
}

t_dlist	*concate_quoted(t_dlist *sub, t_token_type type, t_info *info)
{
	char	*buf;
	t_dlist *begin;
	t_dlist	*new;

	begin = sub;
	sub = sub->next;
	buf = concate_node(&sub, type, info);
	if (!sub)
	{
		ft_memdel((void *)&buf);
		return (begin->next);
	}
	if (begin->next != sub)
	{
		new = dlstnew(ft_malloc_token(buf, literal, 0, ft_strlen(buf)));
		if (!new)
			clear_buffer(buf, 0, info);		
		dlstinsert_node(&info->cmd, sub, new);
	}
	ft_memdel((void *)&buf);
	return (clean_sublst(begin, sub, info));
}

void    parse_quote(t_info *info)
{
	t_dlist *iter;

	iter = info->cmd;
	while (iter)
	{
		if (iter && find_token_type(sgle_quote, iter->content))
			iter = concate_quoted(iter, sgle_quote, info);
		if (iter && find_token_type(dble_quote, iter->content)) // doublon normal permet gerer cas particulier= ''lfa"s"''"':" ou ""lfa's'""'":'
			iter = concate_quoted(iter, dble_quote, info);
		if (iter && find_token_type(sgle_quote, iter->content))
			iter = concate_quoted(iter, sgle_quote, info);
		if (iter)
			iter = iter->next;
	}
	return ;
}