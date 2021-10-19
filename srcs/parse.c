#include "minishell.h"

int		next_identical_token(t_dlist **iter, t_dlist *node, t_token *src)
{
	t_token *token;
	t_token_type type;
	int flag;

	token = (*iter)->content;
	type = token->type;
	flag = 1;
	if ((*iter)->next)
		*iter = (*iter)->next;
	while (*iter)
	{
		token = (*iter)->content;
		if (*iter == node)
			flag = 0;
		if (token->type == type && !flag)
		{
			src->type = literal;
			return (1);
		}
		else if (token->type == type && flag)
			return (0);
		*iter =  (*iter)->next;
	}
	return (0);
}

int		is_it_literal(t_info *info, t_dlist *node, t_token *token)
{
	t_dlist *iter;

	iter = info->cmd;
	while (iter && iter != node)
	{
		if (token->type != dollar &&
			(find_token_type(sgle_quote, iter->content)
			|| find_token_type(dble_quote, iter->content)))
		{
			if (next_identical_token(&iter, node, node->content))
				return (1);
		}
		if (token->type == dollar &&
			find_token_type(sgle_quote, iter->content))
		{
			if (next_identical_token(&iter, node, node->content))
				return (1);
		}
		if (iter)
			iter = iter->next;
	}
	return (0);
}

void	check_pipe(t_info *info)
{
	t_dlist	*tmp;
	t_token	*token;
	t_dlist	*tmp1;
	int i;

	i = info->nbpipe;
	tmp1 = info->cmd;
	while (i)
	{
		tmp1 = info->cmd;
		while (info->cmd)
		{
			token = info->cmd->content;
			if (token->type == pipeline)
			{
				tmp = info->cmd->next;
				if (tmp)
					token = tmp->content;
				while (tmp && token->type != 0 && token->type != literal)
				{
					tmp = tmp->next;
					if (tmp)
						token = tmp->content;
				}
				if (!tmp || (token->type == pipeline && !tmp->next))
				{
					info->nbpipe--;
					info->cmd = info->cmd->prev;
					clear_cmd_lst(&info->cmd->next);
					info->cmd->next = 0;
				}
				else if (token->type == pipeline)
				{
					info->nbpipe--;
					info->cmd->next->prev = 0;
					info->cmd->next = tmp->next;
					tmp->next->prev = info->cmd;
					tmp->next = 0;
					while (tmp->prev)
						tmp = tmp->prev;
					clear_cmd_lst(&tmp);
				}
			}
			info->cmd = info->cmd->next;
		}
		info->cmd = tmp1;
		i--;
	}

}

int		check_error_pipe(t_info	*info)
{
	t_dlist	*tmp;
	t_token	*token;

	if (dlstsize(info->cmd) == 1)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
		clear_cmd_lst(&info->cmd);
		return (1);
	}
	tmp = info->cmd;
	token = tmp->content;
	if (token->type == pipeline)
	{
		tmp = tmp->next;
		token = tmp->content;
		if (token->type == pipeline)
		{
			ft_putstr_fd("bash: syntax error near unexpected token `||'\n", 2);
			clear_cmd_lst(&info->cmd);
			return (1);
		}
	}
	return (0);
}

void	parse_token(t_info *info)
{
	t_dlist *iter;

	iter = info->cmd;
	info->nbpipe = 0;
	while (iter)
	{
		if (find_token_type(pipeline, iter->content))
		{
			if (!is_it_literal(info, iter, iter->content))
				info->nbpipe++;
		}
		if (find_token_type(dollar, iter->content))
			is_it_literal(info, iter, iter->content);
		iter = iter->next;
	}
	//test
	// t_dlist *test;
	// test = info->cmd;
	// t_token *testtok;
	// if (!test)
	// 	printf("NULL cmd\n");
	// if (test)
	// {
	// 	while (test)
	// 	{
	// 		testtok = test->content;
	// 		// printf("check token.value %s\n", (char *)testtok->value);
	// 		printf("check token.value %s\t token.type %d\n", (char *)testtok->value, testtok->type);
	// 		// printf("check token.type %d\n", testtok->type);
	// 		test = test->next;
	// 	}
	// }
	parse_env(info);
	parse_quote(info);


	//printf("nb de pipe = %d\n", info->nbpipe);
	//si pipe => il faut d'abord creer la pipeline de commande puis pipe par pipe expand_env + exec cmd
	
	if (info->nbpipe)
	{
		if (check_error_pipe(info))
			return ;
		check_pipe(info);
	}
	if (info->nbpipe)
	 	create_pipeline(info);
	else
		expand_env(info); // si pas de pipe => expand directement les valeurs puis exec
	return ;
}