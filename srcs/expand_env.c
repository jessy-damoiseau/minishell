#include "minishell.h"

void	concat_node(t_token *atoken, t_token *btoken, t_dlist **iter)
{
	t_token	*token;
	char	*tmp;

	token = (*iter)->content;
	tmp = token->value;
	token->value = ft_strjoin(atoken->value, btoken->value);
	free(tmp);
}

void	concat_narrow_litvalue(t_info *info)
{
	t_dlist	*iter;
	t_dlist	*tmp;

	if (info->nbpipe)
		iter = info->cmdpipe->content;
	else
		iter = info->cmd;
	while (iter)
	{
		if (iter->next)
		{
			if (find_token_type(literal, iter->content)
				&& find_token_type(literal, iter->next->content))
			{
				tmp = iter;
				iter = iter->next;
				concat_node(tmp->content, iter->content, &iter);
				clear_cmd_node(&tmp, info);
			}
		}
		if (iter)
			iter = iter->next;
	}
}

int	is_env_literal(t_dlist **iter)
{
	t_token	*token;

	if (find_token_type(dollar, (*iter)->content))
	{
		if (!(*iter)->next)
			return (1);
		if ((*iter)->next)
		{
			token = (*iter)->next->content;
			if (token->type != literal && token->type != errno_call)
				token = (*iter)->content;
			token->type = literal;
		}
	}
	return (0);
}

void	expand_env(t_info *info)
{
	t_dlist	*iter;

	if (info->nbpipe)
		iter = info->cmdpipe->content;
	else
		iter = info->cmd;
	while (iter)
	{
		if (is_env_literal(&iter))
			return ;
		if (find_errno_type(iter))
			replace_errno(ft_itoa(errno), &iter, info);
		else if (find_token_type(dollar, iter->content))
			find_env_var(&iter, info);
		if (find_errno_type(iter))
			replace_errno(ft_itoa(errno), &iter, info);
		if (iter)
			iter = iter->next;
	}
	concat_narrow_litvalue(info);
	return ;
}
