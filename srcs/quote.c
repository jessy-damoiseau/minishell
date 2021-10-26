#include "minishell.h"

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
	t_dlist	*begin;
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

void	parse_quote(t_info *info)
{
	t_dlist	*iter;

	iter = info->cmd;
	while (iter)
	{
		if (iter && find_token_type(sgle_quote, iter->content))
			iter = concate_quoted(iter, sgle_quote, info);
		if (iter && find_token_type(dble_quote, iter->content))
			iter = concate_quoted(iter, dble_quote, info);
		if (iter && find_token_type(sgle_quote, iter->content))
			iter = concate_quoted(iter, sgle_quote, info);
		if (iter)
			iter = iter->next;
	}
	return ;
}
