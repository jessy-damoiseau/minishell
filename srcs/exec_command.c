#include "minishell.h"

t_list	*fill_gbc(t_list *lst, t_info *info)
{
	t_token *token;
	t_list *tmp;
	t_list *forgbc;
	int i;

	token = lst->content;
	tmp = lst;
	i = 0;
	while (tmp && (token->type != 0
	&& !(token->type >= 3 && token->type <= 6)))
	{
		tmp = tmp->next;
		if (tmp)
			token = tmp->content;
		i++;
	}
	tmp = lst;
	forgbc = tmp;
	while (i-- > 1)
		tmp = tmp->next;
	lst = tmp->next;
	tmp->next = 0;
	lstaddback_gbc(&info->gbc, newgbc(LST, -1, (void *)forgbc));
	if (lst)
	{
		if (lst->next)
			tmp = lst->next;
		lst->next = 0;
		lstaddback_gbc(&info->gbc, newgbc(TOKEN, -1, lst->content));
		free(lst);
		return (tmp);
	}
	return (0);
}

int	check_command(t_info *info)
{
	t_token	*token;
	t_list	*tmp;
	t_list *tmp2;
	char *tmp1;

	tmp = info->cmd;
	while (tmp)
	{
		token = tmp->content;
		tmp1 = token->value;
		//printf ("VALUE : |%s|\n", tmp1);
		if (!ft_strcmp(tmp1, "echo")
		|| !ft_strcmp(tmp1, "cd")
		|| !ft_strcmp(tmp1, "pwd")
		|| !ft_strcmp(tmp1, "export")
		|| !ft_strcmp(tmp1, "unset")
		|| !ft_strcmp(tmp1, "env")
		|| !ft_strcmp(tmp1, "exit"))
		//|| !check_path(tmp1))
			tmp = fill_gbc(tmp, info);
		else if (!ft_strcmp(tmp1, " "))
		{
			//printf("check\n");
			tmp2 = tmp;
			tmp = tmp->next;
			free(tmp2);
		}
		else
		{
			errno = 127;
			return (1);
		}
	}
	return (0);
}

void	exec_command(t_info *info)
{
	t_gbc *tmp;
	int i = 0;
	if (!check_command(info))
	{
		tmp = info->gbc;
		while (tmp)
		{
			printf ("%d:\n", i);
			if (tmp->type == TOKEN)
			{
				t_list *tmp2;
				tmp2 = tmp->str;
				printf("|%s|\n", (char *)tmp2->content);
			}
			else if (tmp->type == LST)
			{
				t_list *tmp2;
				tmp2 = tmp->str;
				while (tmp2)
				{
					t_token *tmp3;
					tmp3 = tmp2->content;
					printf("|%s|\n", (char *)tmp3->value);
					tmp2 = tmp2->next;
				}
			}
			tmp = tmp->next;
			i++;
		}
		//printf("check\n");
			// check_pipe_rdct(info);
			// if (!check_builtins())
			// else if (!check_exec())
			// else
			// check_other();
	}
	//printf("check2\n");
}
