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

int		createfile(t_dlist *allfile, t_info *info)
{
	t_token	*token;
	t_dlist *tmp;
	int fd;
	char *str;
	struct stat sb;

	str = 0;
	int i;
	info->dlb_redir_left_str = 0;
	info->redir_left = 0;
	i = 1;
	while (allfile)
	{
		tmp = allfile->content;
		token = tmp->content;
		if (token->type != 4 && token->type != 6)
		{
			while (tmp)
			{
				token = tmp->content;
				tmp = tmp->next;
			}
			printf("\n");
			if (stat((char *)token->value, &sb))
				{
					fd = open((char *)token->value, O_CREAT, 0664);
					if (fd < 0)
						ft_exit(0, info, err_fd);
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
		tmp = allfile->content;
		token = tmp->content;
		if (token->type == 4)
		{
			if (info->dlb_redir_left_str)
				ft_lstclear(&info->dlb_redir_left_str, &ft_memdel);
			while (tmp->next)
				tmp = tmp->next;
			token = tmp->content;
			if (check_exist((char *)token->value))
			{
				ft_putstr_fd("bash: ", 2);
				ft_putstr_fd((char *)token->value, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				errno = 1;
				return (1);
			}
			info->redir_left = i;
		}
		if (token->type == 6)
		{
			if (info->dlb_redir_left_str)
				ft_lstclear(&info->dlb_redir_left_str, &ft_memdel);
			while (tmp->next)
				tmp = tmp->next;
			token = tmp->content;
			while (!str || ft_strcmp(str, (char *)token->value))
			{
				ft_putstr_fd(">", 1);
				get_next_line(1, &str);
				ft_putstr_fd("\n", 1);
				if (ft_strcmp(str, (char *)token->value))
					ft_lstadd_back(&info->dlb_redir_left_str, ft_lstnew(str));
			}
			info->redir_left = i;
		}
		if (token->type == 3)
		{
			while (tmp->next)
				tmp = tmp->next;
			token = tmp->content;
			fd = open((char *)token->value, O_TRUNC);
			close(fd);
		}

		i++;
		allfile = allfile->next;
	}
	return (0);
}

t_dlist	*clear_struct(t_dlist **lst, t_info *info)
{
	int i;
	int len;
	t_dlist *tmp;
	t_dlist *tmp2;
	t_token *token;

	i = 0;
	tmp = 0;
	len = dlstsize(*lst);
	if (info->redir_left)
	{
		while (++i < info->redir_left)
			*lst = (*lst)->next;
		tmp2 = (*lst)->content;
		dlstadd_back(&tmp, dlstnew(tmp2));
	}
	while((*lst)->next)
		*lst = (*lst)->next;
	tmp2 = (*lst)->content;
	token = tmp2->content;
	if (!info->redir_left || len >= 2)
	{
		while ((token->type == 4 || token->type == 6))
		{
			*lst = (*lst)->prev;
			tmp2 = (*lst)->content;
			token = tmp2->content;
		}
		dlstadd_back(&tmp, dlstnew(tmp2));
	}
	while ((*lst)->prev)
		*lst = (*lst)->prev;
	return (tmp);

}

void	go_redirect(t_dlist *rdrct, t_info *info, t_dlist *mcmd)
{
	int fdin;
	int fdout;
	int fd1;
	int fd0;
	int	fd[2];
	t_dlist *tmp;
	t_token	*token;
	t_list	*dblrleft;

	fd1 = dup(1);
	fd0 = dup(0);
	if (dlstsize(rdrct) == 2)
	{
		while (rdrct)
		{
			tmp = rdrct->content;
			token = tmp->content;
			if (token->type == 6)
			{
				
			}
			else
			{
				while (tmp->next)
					tmp = tmp->next;
				token = tmp->content;
				if (rdrct->next)
					fdin = open((char *)token->value, O_RDONLY);
				else
					fdout = open((char *)token->value, O_WRONLY | O_APPEND);
			}
			rdrct = rdrct->next;
		}
		if (fdin >= 0)
			dup2(fdin, 0);
		if (fdout >= 0)
			dup2(fdout, 1);
		if (check_builtins(info, mcmd))
			check_exec(info, mcmd);
	}
	else
	{
		tmp = rdrct->content;
		token = tmp->content;
		if (token->type == 6)
		{
			pipe(fd);
			dblrleft = info->dlb_redir_left_str;
			while (dblrleft)
			{
				ft_putstr_fd(dblrleft->content, fd[1]);
				ft_putstr_fd("\n", fd[1]);
				dblrleft = dblrleft->next;
			}
			dup2(fd[0], 0);
			close(fd[1]);
		}
		else if (token->type == 4)
		{
			while (tmp->next)
				tmp = tmp->next;
			token = tmp->content;
			fdin = open((char *)token->value, O_RDONLY);
			dup2(fdin, 0);
		}
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			token = tmp->content;
			fdout = open((char *)token->value, O_WRONLY | O_APPEND);
			if (fdout < 0)
				printf("probleme\n");
			dup2(fdout, 1);
		}
		if (check_builtins(info, mcmd))
			check_exec(info, mcmd);
		else
		{
			printf("test\n");
			close(fd[0]);
		}
		
	}
	dup2(fd1, 1);
	dup2(fd0, 0);
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


		t_dlist *tmp;
		tmp = stockcmd->content;
		printf("cmd:\n");
		while (tmp)
		{
			token = tmp->content;
			printf("%s", (char*)token->value);
			tmp = tmp->next;
		}
		printf("\n");


		if (createfile(stock_rdrct, info))
		{
			dlstclear(&stock_rdrct, &ft_memdel);
			dlstclear(&stockcmd, &ft_memdel);
			return ;
		}
		go_redirect(clear_struct(&stock_rdrct, info), info, stockcmd);
	}
}
