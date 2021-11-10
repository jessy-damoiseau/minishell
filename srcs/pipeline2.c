/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jessy <jessy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 18:26:49 by jessy             #+#    #+#             */
/*   Updated: 2021/11/10 01:16:51 by jessy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_pipe3(t_dlist **tmp, t_dlist **lr, t_dlist **pipe)
{
	(*tmp) = (*pipe)->prev;
	(*tmp)->next = 0;
	(*pipe)->prev = 0;
	(*tmp) = (*lr)->next;
	(*tmp)->prev = 0;
	clear_cmd_lst(tmp);
	(*lr)->next = 0;
	while ((*lr)->prev)
		(*lr) = (*lr)->prev;
	dlstadd_back(&g_info.cmdpipe, dlstnew((*lr)));
}

void	create_pipe2(t_dlist **pipe)
{
	t_dlist	*lr;
	t_token	*token;
	t_dlist	*tmp;

	lr = (*pipe)->prev;
	token = lr->content;
	while (token->type == space)
	{
		lr = lr->prev;
		token = lr->content;
	}
	(*pipe) = (*pipe)->next;
	token = (*pipe)->content;
	while (token->type == space)
	{
		(*pipe) = (*pipe)->next;
		token = (*pipe)->content;
	}
	create_pipe3(&tmp, &lr, pipe);
}

void	create_pipeline(void)
{
	t_dlist	*pipe;
	t_token	*token;

	pipe = g_info.cmd;
	while (pipe->next)
	{
		token = pipe->content;
		if (token->type == pipeline)
			create_pipe2(&pipe);
		else
			pipe = pipe->next;
	}
	while (pipe->prev)
		pipe = pipe->prev;
	dlstadd_back(&g_info.cmdpipe, dlstnew(pipe));
	pipe = g_info.cmdpipe;
	while (g_info.cmdpipe)
	{
		expand_env();
		g_info.cmdpipe = g_info.cmdpipe->next;
	}
	g_info.cmdpipe = pipe;
	exec_pipeline(g_info.cmdpipe, 0, 0, -1);
}

void	check_if_exit(t_dlist *list)
{
	t_dlist	*tmp;
	t_token	*token;

	while (list->next)
		list = list->next;
	tmp = list->content;
	if (!tmp)
		return ;
	token = tmp->content;
	if (!strcmp((char *)token->value, "exit"))
	{
		tmp = tmp->next;
		if (tmp)
			check_if_exit2(tmp);
	}
}

void	check_if_exit2(t_dlist	*tmp)
{
	char	*str;
	t_token	*token;
	int		i;

	i = 0;
	token = tmp->content;
	if (token->type == space)
	{
		tmp = tmp->next;
		if (tmp)
		{
			token = tmp->content;
			str = token->value;
			while (ft_isdigit(str[i]))
				i++;
			if (!str[i] == '\0')
				errno = ft_atoi(str) % 256;
			else
				errno = 2;
			if (tmp->next)
				errno = 1;
		}
	}
}
