#include "minishell.h"


void    create_pipeline(t_info *info, int pipe)
{
	// t_dlist **iter;
	t_dlist *it;
	int i;

	it = info->cmd;
	info->pipe = malloc(sizeof(int *) * pipe);
	// printf("check1\n");
	i = 0;
	while (it && i < pipe)
	{
		if (find_token_type(pipeline, it->content))
			info->pipe[i] = it;
			// dlstadd_back(info->pipe, it);
		it = it->next;
		i++;
	}
	// test //
	t_token *token;

	i = 0;
	t_dlist *test;
	while (i < pipe)
	{
		test = info->pipe[i];
		token = test->content;
		printf("pipeline = %s\n", (char *)token->value);
		i++;
		// test = test->next;
	}
	// iter = info->pipe;
	// 
	// printf("check2\n");
	// while (*iter)
	// {
	// 	token = (*iter)->content;
	// 	printf("pipeline = %s\n", (char *)token->value);
	// 	*iter = (*iter)->next;
	// }
	return ;
}