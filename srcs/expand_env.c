#include "minishell.h"

static void	replace_node_value(char *evar, t_dlist **iter)
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

static int	find_env_var(t_dlist **iter, t_info *info)
{
	t_dlist	*tmp;
	t_list	*env;
	t_token	*token;

	tmp = *iter;
	*iter = (*iter)->next;
	token = (*iter)->content;
	if (info->env)
	{
		env = info->env;
		while (env)
		{
			if (!ft_strncmp(env->content, token->value,
					ft_strlen(token->value)))
			{
				replace_node_value(env->content, iter);
				clear_cmd_node(&tmp, info);
				return (1);
			}
			env = env->next;
		}
	}
	clear_cmd_node(&tmp, info);
	clear_cmd_node(iter, info);
	return (0);
}

int	find_errno_type(t_dlist *lst)
{
	t_token *token;

	token = lst->content;
	if (token->type == dollar && lst->next)
	{
		lst = lst->next;
		token = lst->content;
		if (!strcmp((char *)token->value, "?"))
			return (1);
	}
	return (0);
}

void	replace_errno(char *str, t_dlist **lst, t_info *info)
{
	t_dlist *tmp;
	t_token *token;

	tmp = (*lst)->next;
	(*lst)->next = tmp->next;
	clear_cmd_node(&tmp, info);
	token = (*lst)->content;
	token->type = literal;
	free(token->value);
	token->value = ft_strdup(str);
	free(str);
}

void	concat_node(t_token *atoken, t_token *btoken, t_dlist **iter)
{
	t_token	*token;

	token = (*iter)->content;
	char *tmp = token->value;
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
			if (find_token_type(literal, iter->content) &&
				find_token_type(literal, iter->next->content))
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
		if (find_errno_type(iter)) // permet traiter le cas echo $SHLVLs$?
			replace_errno(ft_itoa(errno), &iter, info);
		if (iter)
			iter = iter->next;
	}
	concat_narrow_litvalue(info);
	return ;
}