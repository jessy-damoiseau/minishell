#include "minishell.h"

void 	clear_token(t_dlist *list, t_token *token)
{
	ft_memdel(&token->value);
	ft_memdel(&list->content);
	ft_memdel((void *)&list);
	// free(list);
}

void	clear_cmd_lst(t_dlist **lst)
{
	t_dlist	*tmp;
	t_token	*token;

	tmp = NULL;
	token = NULL;
	if (*lst)
	{
		while (*lst)
		{
			tmp = *lst;
			token = tmp->content;
			*lst = (*lst)->next;
			clear_token(tmp, token);
		}
	}
	*lst = NULL;
}

void	free_dbl(char **str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	while(i--)
		free(str[i]);
	free(str);
	str = 0;

}

// cette fonction peut etre amelioree avec un tab msg erreur vs enum code
// et une fonction qui fait la correspondance comme ft_define_token
void	ft_exit(t_info *info, t_err_code err_code)
{
    if (err_code)
    {
		clear_cmd_lst(&info->cmd);
		ft_lstclear(&info->env, &ft_memdel);
		ft_putstr_fd("Error : Malloc failed\n", 2);
		exit (1);
    }
    else
    {
		clear_cmd_lst(&info->cmd);
		ft_lstclear(&info->env, &ft_memdel);
		ft_putstr_fd("You exited minishell prog\n", 1);
		exit (0);
	}
}