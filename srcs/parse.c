#include "minishell.h"

int	next_identical_token(t_dlist **iter, t_dlist *node, t_token *src)
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

int is_it_literal(t_info *info, t_dlist *node, t_token *token)
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
	parse_env(info); // gere les $ entre sgle quote
	parse_quote(info); // concat les quote


	//printf("nb de pipe = %d\n", info->nbpipe);
	 // si pipe => il faut d'abord creer la pipeline de commande puis pipe par pipe expand_env + exec cmd
	if (info->nbpipe)
	 	create_pipeline(info);
	else
		expand_env(info); // si pas de pipe => expand directement les valeurs puis exec
	return ;
}