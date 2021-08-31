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

void	expand_env(t_info *info)
{
	t_dlist *iter;

	if (info->nbpipe)
		iter = info->cmdpipe->content;
	else
		iter = info->cmd;
	while (iter)
	{
		if (find_token_type(dollar, iter->content))
		{
			find_env_var(&iter, info);
		}
		if (iter)
			iter = iter->next;
	}
	return ;
}