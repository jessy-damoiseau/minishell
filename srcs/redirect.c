#include "minishell.h"

int		check_redirecterr(t_gbc *allcmd)
{
	t_list *tmp;
	//t_gbc *tmp2;

	while (allcmd)
	{
		if (allcmd->type == STR)
			if (allcmd->next->type == STR)
			{
				tmp = allcmd->next->str;
				ft_putstr_fd("bash: syntax error near unexpected token `", 2);
				ft_putstr_fd((char *)tmp->content, 2);
				ft_putstr_fd("'\n", 2);
				return (1);
			}
		allcmd = allcmd->next;
	}
	return (0);
}

void	redirection(t_info *info)
{
	if (!check_redirecterr(info->gbc))
	{

	}
}