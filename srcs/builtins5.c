/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jessy <jessy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 01:02:06 by jessy             #+#    #+#             */
/*   Updated: 2021/10/28 01:02:26 by jessy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_export(char *cmd, t_info *info, int fd)
{
	int	i;

	i = 6;
	while (cmd[i] && cmd[i] == ' ')
		i++;
	if (!cmd[i])
		export1(info, fd);
	else
		export2(cmd, i, info);
}
