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
	//int fd;
	//int pfd[2];
	(void)cmd, (void)stock_rdrct, (void)info;
}
/* < */
void	rdrctsgll(t_dlist *cmd, t_dlist *stock_rdrct, t_info *info)
{
	(void)cmd, (void)stock_rdrct, (void)info;
}
/* >> */
void	rdrctdblr(t_dlist *cmd, t_dlist *stock_rdrct, t_info *info)
{
	(void)cmd, (void)stock_rdrct, (void)info;
}
/* << */
void	rdrctdbll(t_dlist *cmd, t_dlist *stock_rdrct, t_info *info)
{
	(void)cmd, (void)stock_rdrct, (void)info;
}

int	createfile(t_dlist *allfile)
{
	t_token	*token;
	t_dlist *tmp;
	int fd;
	struct stat sb;

	while (allfile)
	{
		tmp = allfile->content;
		token = tmp->content;
		printf("type ->: %d\n", token->type);
		if (token->type != 4 && token->type != 6)
		{
			while (tmp)
			{
				token = tmp->content;
				printf("file ->: %s\n", (char *)token->value);
				tmp = tmp->next;
			}
			printf("\n");
			if (stat((char *)token->value, &sb))
				{
					fd = open((char *)token->value, O_CREAT, 0664);
					if (fd < 0)
						return (1);
					close(fd);
				}
			else
			{
				if (opendir((char *)token->value))
				{
					ft_putstr_fd("bash: ", 2);
					ft_putstr_fd((char *)token->value, 2);
					ft_putstr_fd(": Is a directory", 2);
					errno = 1;
					return (1);
				}
			}
		}
		allfile = allfile->next;
	}
	return (0);
}

void	redirection(t_info *info)
{
	t_dlist *cmd;
	t_dlist	*stockcmd;
	t_gbc	*tmpgbc;
	t_dlist	*stock_rdrct;
	t_token	*token;

	stockcmd = 0;
	stock_rdrct = 0;
	if (!check_redirecterr(info->gbc))
	{
		tmpgbc = info->gbc;
		while (tmpgbc)
		{
			cmd = tmpgbc->str;
			token = cmd->content;
			if (token->type >= 3 && token->type <= 6)
				dlstadd_back(&stock_rdrct, dlstnew(cmd));
			else
				dlstadd_back(&stockcmd, dlstnew(cmd));
			tmpgbc = tmpgbc->next;
		}
		printf("check\n");
		if (createfile(stock_rdrct))
		{
			//all free;
			return ;
		}
		printf("check2\n");

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
