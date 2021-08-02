#include "minishell.h"

int		check_redirecterr(t_gbc *allcmd)
{
	t_dlist	*tmp;
	t_token	*token;

	while (allcmd)
	{
		tmp = allcmd->str;
		token = tmp->content;
		if (token->type >= 3 && token->type <= 6)
		{
			token = tmp->next->content;
			if (token->type >= 3 && token->type <= 6)
			{
				ft_putstr_fd("bash: syntax error near unexpected token `", 2);
				ft_putstr_fd((char *)token->value, 2);
				ft_putstr_fd("'\n", 2);
				errno = 2;
				return (1);
			}
		}
		allcmd = allcmd->next;
	}
	return (0);
}
/* > */
void	rdrctsglr(t_dlist *cmd, t_dlist *stock_rdrct, t_info *info)
{

}
/* < */
void	rdrctsgll(t_dlist *cmd, t_dlist *stock_rdrct, t_info *info)
{

}
/* >> */
void	rdrctdblr(t_dlist *cmd, t_dlist *stock_rdrct, t_info *info)
{

}
/* << */
void	rdrctdbll(t_dlist *cmd, t_dlist *stock_rdrct, t_info *info)
{

}

void	redirection(t_info *info)
{
	t_dlist	*cmd;
	t_gbc	*tmpgbc;
	t_dlist	*stock_rdrct;
	t_token	*token;

	if (!check_redirecterr(info->gbc))
	{
		tmpgbc = info->gbc;
		cmd = tmpgbc->str;
		token = cmd->content;
		if (token->type >= 3 && token->type <= 6)
		{
			stock_rdrct = cmd;
			cmd = cmd->next;
		}
		else
		{
			stock_rdrct = cmd->next;
		}
		token = stock_rdrct->content;
		if (token->type == 3)
			rdrctsglr(cmd, stock_rdrct, info);
		else if(token->type == 4)
			rdrctsgll(cmd, stock_rdrct, info);
		else if (token->type == 5)
			rdrctdblr(cmd, stock_rdrct, info);
		else
			rdrctdbll(cmd, stock_rdrct, info);
	}
}