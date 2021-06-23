#include "minishell.h"

void	ft_clear_token(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;
	t_token	*token;

	tmp = NULL;
	token = NULL;

	while (*lst)
	{
		tmp = *lst;
		token = tmp->content;
		*lst = (*lst)->next;
		del(token->value);
		del(tmp->content);
		free(tmp);
	}
	*lst = NULL;
}

// cette fonction peut etre amelioree avec un tab msg erreur vs enum code
// et une fonction qui fait la correspondance comme ft_define_token
void    ft_exit(t_info *info, t_err_code err_code)
{
    if (err_code)
    {
		ft_clear_token(&info->cmd, &ft_memdel);
		ft_lstclear(&info->env, &ft_memdel);
		ft_putstr_fd("Error : Malloc failed\n", 2);
		exit (1);
    }
    else
    {
		ft_clear_token(&info->cmd, &ft_memdel);
		ft_lstclear(&info->env, &ft_memdel);
		ft_putstr_fd("You exited minishell prog\n", 1);
		exit (0);
    }
}