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

void	replace_errno(char *str, t_dlist **lst)
{
	t_dlist *tmp;
	t_token *token;


	tmp = (*lst)->next;
	(*lst)->next = tmp->next;
	tmp->prev = 0;
	tmp->next = 0;
	clear_cmd_lst(&tmp);
	token = (*lst)->content;
	token->type = literal;
	free(token->value);
	token->value = ft_strdup(str);
	free(str);
}

void	expand_env(t_info *info)
{
	t_dlist *iter;

	if (info->nbpipe)
		iter = info->cmdpipe->content;
	else
		iter = info->cmd;
	while (iter)
	{
		if (find_errno_type(iter))
			replace_errno(ft_itoa(errno), &iter);
		else if (find_token_type(dollar, iter->content))
			find_env_var(&iter, info);
		if (iter)
			iter = iter->next;
	}
	return ;
}