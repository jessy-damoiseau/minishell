/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 02:30:46 by jessy             #+#    #+#             */
/*   Updated: 2021/11/07 20:49:55 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	createfile2(t_dlist *tmp, t_token *token)
{
	struct stat	sb;
	DIR			*check;

	while (tmp && token->type != 9)
	{
		token = tmp->content;
		tmp = tmp->next;
	}
	if (stat((char *)token->value, &sb))
	{
		if (createfile2b(token))
			return (1);
	}
	else
	{
		check = opendir((char *)token->value);
		if (check)
			return (createfile2err(token, check));
	}
	return (0);
}

int	createfile3(t_dlist *tmp, t_token *token, int i)
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
	return (0);
}

// EN COURS
char	*concat_multiple_str(char **split, char *last)
{
	int i;
	char *tmp2;
	char *tmp3;

	i = 0;
	if (!split || !split[i])
		return (0);
	tmp2 = ft_strdup(split[i]);
	tmp3 = tmp2;
	printf("check split3 = %s\n", tmp2);
	i++;
	while (split[i])
	{
		tmp3 = ft_strjoin(tmp2, split[i]);
		printf("check concat loop = %s\n", tmp3);
		free(tmp2);
		tmp2 = tmp3;
		i++;
	}
	printf("check concat = %s\n", tmp2);
	if (last)
	{
		tmp3 = ft_strjoin(tmp2, last);
		free(tmp2);
		tmp2 = tmp3;
	}
	return (tmp3);
}

char	**inputval_to_expand(char *str)
{
	int		i;
	char	**tab;

	i = 0;
	tab = malloc(sizeof(char *) * 2);
	if (!tab)
		return (0);
	while (str[i] && valid_env_char(str[i]))
		i++;
	tab[0] = ft_strndup(str, i);
	tab[1] = ft_strndup(str + i, (ft_strlen(str) - i));
	return (tab);
}

int	expand_input_env(char **s)
{
	char	**tab;
	char	*tmp;
	t_list	*env;

	tmp = *s;
	tab = inputval_to_expand(*s);
	if (g_info.env)
	{
		env = g_info.env;
		while (env)
		{
			if (check_if_value_in_env(env->content, tab[0]))
			{
				printf("check env = %s\n", (char *)env->content);
				*s = ft_strjoin(env->content +
					(ft_strlen_utils(env->content, '=') + 1), tab[1]);
				free(tmp);
				return (clean_tab(tab, 1));
			}
			env = env->next;
		}
		*s = ft_strdup("");
		free(tmp);
	}
	return (clean_tab(tab, 0));
}

void	expand_dlb_left_input(char **str)
{
	char	**split;
	char	*tmp;
	int		i;

// test
	{
		char **tab = ft_split(*str, '$');
		for(int i = 0; tab[i]; i++)
			printf("check input = %s\n", tab[i]);
		for(int i = 0; tab[i]; i++)
			free(tab[i]);
		free(tab);
	}
	tmp = NULL;
	i = ft_strlen(*str);
	if (!ft_strncmp(*str + (i - 1), "$", 1))
		tmp = "$";
	split = ft_split(*str, '$');
	i = 0;
	char *stmp;
	while (split[i])
	{
		printf("check split = %c\n", *split[i]);
		if (*split[i] == '?')
		{
			stmp =  split[i];
			char *errvalue = ft_itoa(errno);
			split[i] = ft_strjoin(errvalue, stmp + 1);
			printf("check split2 = %s\n", split[i]);
			free(errvalue);
			free(stmp);
		}
		else
			expand_input_env(&split[i]);
		i++;
	}
	free(*str); // correction de leak
	*str = concat_multiple_str(split, tmp);
	printf("check concat2 = %s\n", *str);
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int	createfile4(t_dlist *tmp, t_token *token, int i)
{
	char	*str;

	str = 0;
	if (g_info.dlb_redir_left_str)
		ft_lstclear(&g_info.dlb_redir_left_str, &ft_memdel);
	while (tmp->next)
		tmp = tmp->next;
	token = tmp->content;
	g_info.gnl = 0;
	g_info.stop = 0;
	while (!str || ft_strcmp(str, (char *)token->value))
	{
		g_info.gnl = 1;
		ft_putstr_fd("> ", 1);
		str = readline("");
		if (g_info.stop)
			return (createfilestop(str));
		if (ft_strrchr(str, '$'))
			expand_dlb_left_input(&str);
		ft_lstadd_back(&g_info.dlb_redir_left_str, ft_lstnew(str));
	}
	g_info.redir_left = i;
	return (0);
}

void	createfile5(t_dlist *tmp, t_token *token)
{
	int	fd;

	while (tmp->next)
		tmp = tmp->next;
	token = tmp->content;
	fd = open((char *)token->value, O_TRUNC);
	g_info.redir_right = 1;
	close(fd);
}

int	createfileb(t_dlist *tmp, t_token *token, int i)
{
	if (token->type == 4)
		if (createfile3(tmp, token, i))
			return (1);
	if (token->type == 6)
		if (createfile4(tmp, token, i))
			return (1);
	if (token->type == 3)
		createfile5(tmp, token);
	if (token->type == 5)
		g_info.redir_right = 1;
	return (0);
}
