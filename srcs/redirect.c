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
			if (tmp->next)
				token = tmp->next->content;
			if (!tmp->next || (token->type >= 3 && token->type <= 6))
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

int	check_directory(char *str)
{
	char	**dir;
	int i;
	int	j;

	j = 0;
	dir = ft_split(str, '/');
	while (dir[j])
		j++;
	if (j <= 1)
	{
		free_dbl(dir);
		return (0);
	}
	i = 0;
	while (dir[i] && i < j - 1)
	{
		if (check_exist(dir[i]))
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(str, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			free_dbl(dir);
			errno = 1;
			return (1);
		}
		i++;

	}
	free_dbl(dir);
	return (0);
}

int		createfile(t_dlist *allfile)
{
	t_token	*token;
	t_dlist *tmp;
	int fd;
	char *str;
	int i;
	struct stat sb;
	DIR *check;

	str = 0;
	i = 1;

	while (allfile)
	{
		tmp = allfile->content;
		token = tmp->content;
		if (token->type != 4 && token->type != 6)
		{
			while (tmp && token->type != 9)
			{
				token = tmp->content;
				tmp = tmp->next;
			}
			if (stat((char *)token->value, &sb))
				{
					if (check_directory((char *)token->value))
						return (1);
					fd = open((char *)token->value, O_CREAT, 0664);
					if (fd < 0)
						ft_exit(0, err_fd);
					close(fd);
					g_info.redir_right = 1;
					
				}
			else
			{
				check = opendir((char *)token->value);
				if (check)
				{
					ft_putstr_fd("bash: ", 2);
					ft_putstr_fd((char *)token->value, 2);
					ft_putstr_fd(": Is a directory", 2);
					closedir(check);
					errno = 1;
					return (1);
				}
			}
		}
		tmp = allfile->content;
		token = tmp->content;
		if (token->type == 4)
		{
			if (g_info.dlb_redir_left_str)
				ft_lstclear(&g_info.dlb_redir_left_str, &ft_memdel);
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
			g_info.redir_left = i;
		}
		if (token->type == 6)
		{
			if (g_info.dlb_redir_left_str)
				ft_lstclear(&g_info.dlb_redir_left_str, &ft_memdel);
			while (tmp->next)
				tmp = tmp->next;
			token = tmp->content;
///////////////////////////////////////////////////////////////
			g_info.gnl = 0;
			g_info.stop = 0;
			while (!str || ft_strcmp(str, (char *)token->value))
			{
				g_info.gnl = 1;
				ft_putstr_fd("> ", 1);
				str = readline("");
				if (g_info.stop)
				{
					ft_lstclear(&g_info.dlb_redir_left_str, &ft_memdel);
					free(str);
					return (1);
				}
				ft_lstadd_back(&g_info.dlb_redir_left_str, ft_lstnew(str));
			}
			g_info.redir_left = i;
		}
////////////////////////////////////////////////////////////////////////////

		if (token->type == 3)
		{
			while (tmp->next)
				tmp = tmp->next;
			token = tmp->content;
			fd = open((char *)token->value, O_TRUNC);
			g_info.redir_right = 1;
			close(fd);
		}
		if (token->type == 5)
			g_info.redir_right = 1;
		i++;
		allfile = allfile->next;
	}
	return (0);
}

t_dlist	*clear_struct(t_dlist **lst)
{
	int i;
	t_dlist *tmp;
	t_dlist *tmp2;
	t_token *token;

	i = 0;
	tmp = 0;
	if (g_info.redir_left)
	{
		while (++i < g_info.redir_left)
			*lst = (*lst)->next;
		tmp2 = (*lst)->content;
		dlstadd_back(&tmp, dlstnew(tmp2));
	}
	while((*lst)->next)
		*lst = (*lst)->next;
	tmp2 = (*lst)->content;
	token = tmp2->content;
	if (g_info.redir_right)
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

void	go_redirect(t_dlist *rdrct, t_dlist *mcmd)
{
	int fdin;
	int fdout;
	int fd1;
	int fd0;
	int	fd[2];
	t_dlist *tmp;
	t_dlist	*tmp2;
	t_token	*token;
	t_list	*dblrleft;

	fd1 = dup(1);
	fd0 = dup(0);
	tmp2 = rdrct;
	if (dlstsize(rdrct) == 2)
	{
		fdin = -1;
		fdout = -1;
		while (rdrct)
		{
			tmp = rdrct->content;
			token = tmp->content;
			if (token->type == 6)
			{
				pipe(fd);
				dblrleft = g_info.dlb_redir_left_str;
				while (dblrleft->next)
				{
					ft_putstr_fd(dblrleft->content, fd[1]);
					ft_putstr_fd("\n", fd[1]);
					dblrleft = dblrleft->next;
				}
				dup2(fd[0], 0);
				close(fd[1]);
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
		if (check_builtins(mcmd))
			check_exec(mcmd->content);
		
	}
	else
	{
		tmp = rdrct->content;
		token = tmp->content;
		if (token->type == 6)
		{
			pipe(fd);
			dblrleft = g_info.dlb_redir_left_str;
			while (dblrleft->next)
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
			fdin = open((char *)token ->value, O_RDONLY);
			dup2(fdin, 0);
		}
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			token = tmp->content;
			fdout = open((char *)token->value, O_WRONLY | O_APPEND);
			dup2(fdout, 1);
		}
		if (mcmd)
			if (check_builtins(mcmd))
				check_exec(mcmd->content);
	}
	dup2(fd1, 1);
	dup2(fd0, 0);
	tmplstclear(&tmp2);
	ft_lstclear(&g_info.dlb_redir_left_str, &ft_memdel);
}

void	redirection(void)
{
	t_dlist *cmd;
	t_dlist	*stockcmd;
	t_gbc	*tmpgbc;
	t_dlist	*stock_rdrct;
	t_token	*token;

	stockcmd = 0;
	stock_rdrct = 0;
	if (!check_redirecterr(g_info.gbc))
	{
		tmpgbc = g_info.gbc;
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
		if (createfile(stock_rdrct))
		{
			tmplstclear(&stock_rdrct);
			tmplstclear(&stockcmd);
			return ;
		}
		go_redirect(clear_struct(&stock_rdrct), stockcmd);
		tmplstclear(&stock_rdrct);
		tmplstclear(&stockcmd);
	}
}
