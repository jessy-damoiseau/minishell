#include "minishell.h"

void	clear_unwanted_quotes(t_dlist **lst, t_info *info)
{
	t_dlist *tmp;

	tmp = *lst;
	if (!(*lst)->prev)
	{
		*lst = (*lst)->next;
		(*lst)->prev = NULL;
		info->cmd = *lst;
	}
	else if (!(*lst)->next)
	{
		*lst = (*lst)->prev;
		tmp->prev->next = NULL;
	}
	else if ((*lst)->prev && (*lst)->next)
	{
		*lst = (*lst)->next;
		(*lst)->prev = tmp->prev;
		tmp->prev->next = *lst;

	}
	clear_token(tmp, tmp->content);
}

int	find_quotes(t_dlist **iter, t_dlist *node, t_info *info)
{
	int flag;
	t_dlist *tmp;
	t_token	*token;

	tmp = *iter;
	flag = 1;
	if ((*iter)->next)
		*iter = (*iter)->next;
	while (*iter)
	{
		token = (*iter)->content;
		if (*iter == node)
			flag = 0;
		if (token->type == dble_quote && !flag)
		{
			clear_unwanted_quotes(iter, info);
			clear_unwanted_quotes(&tmp, info);
			return (1);
		}
		else if (token->type == dble_quote && flag)
			return (0);
		*iter = (*iter)->next;
	}
	return (0);
}

void	delete_dble_quotes(t_info *info, t_dlist *node)
{
	t_dlist *iter;

	iter = info->cmd;
	while (iter && iter != node)
	{
		if (find_token_type(dble_quote, iter->content))
		{
			 find_quotes(&iter, node, info);
		}
		if (iter)
			iter = iter->next;
	}
}

void	parse_env(t_info *info)
{
	t_dlist *iter;

	iter = info->cmd;
	while (iter)
	{
		if (find_token_type(dollar, iter->content))
			delete_dble_quotes(info, iter);
		iter = iter->next;
	}
}

// void	expand_env(t_info *info)
// {

// 	return ;
// }