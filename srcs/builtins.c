#include "minishell.h"

void    pwd(t_info *info, char *cmd, int fd)
{
	if (!ft_strcmp(cmd, "pwd"))
	{
		ft_putstr_fd(info->pwd, fd);
		write(fd, "\n", 1);
		errno = 0;
	}
	else
	{
		ft_putstr_fd("pwd: too many arguments\n", 2);
		errno = 1;
	}
}

void	cd(char *cmd)
{
	int i;
	int j;
	char *str;

	i = 2;
	j = 0;
	str = getenv("HOME");
	if (cmd[2] != '\0')
	{
		while (cmd[++i])
			if (cmd[i] == ' ')
			{
				j = i;
				while (cmd[j] == ' ')
					j++;
				if (cmd[j] == '\0')
					break;
				errno = 1;
				ft_putstr_fd("cd: too many argument\n", 2);
				return ;
			}
		if (!ft_strcmp(&cmd[3], "~") || j != 0)
			chdir(str);
		else
		{
			if (chdir(&cmd[3]) == -1)
			{
				ft_putstr_fd("cd: ", 2);
				ft_putstr_fd(cmd, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				errno = 1;
			}
		}
	}
	else
	{
		chdir(str);
		errno = 0;
	}
	printf("errno :|%d|\n", errno);
}

void	env(char *cmd, t_info *info, int fd)
{
	t_list *tmp;
	tmp = info->env;
	int i;
	struct stat sb;

	i = 3;
	while (cmd[i] == ' ')
		i++;
	if (ft_strcmp(cmd, "env") && cmd[i])
	{
		i  = 3;
		while (cmd[i] == ' ')
			i++;
		ft_putstr_fd("env: '", 2);
		ft_putstr_fd(&cmd[i], 2);
		if (!stat(&cmd[i], &sb))
		{
			ft_putstr_fd("’: Permission denied\n", 2);
			errno = 126;
		}
		else
		{
			ft_putstr_fd("’: No such file or directory\n", 2);
			errno = 127;
		}
	}
	else
	{
		while (tmp)
		{
			ft_putstr_fd(tmp->content, fd);
			ft_putstr_fd("\n", fd);
			tmp = tmp->next;
		}
		errno = 0;
	}
}

void	unset(char *cmd, t_info *info)
{
	t_list *tmp;
	t_list *tmp2;
	char *tfchier;
	int i;
	int j;

	i = 5;
	j = -1;
	tmp = info->env;
	while (cmd[i] == ' ')
		i++;
	while (cmd[i + ++j])
		if (!ft_isalnum(cmd[i + j]) && cmd[i + j] != ' ' && cmd[i + j] != '_')
		{
			ft_putstr_fd("unset: '", 2);
			ft_putstr_fd(&cmd[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			errno = 1;
			return ;
		}
	while (tmp->next)
	{
		printf("i: %d, j: %d, &cmd[i]: %s\n", i, j, &cmd[i]);
		tfchier = tmp->next->content;
		if (!ft_strncmp(tmp->next->content, &cmd[i], j) && (tfchier[j] == '=' || !tfchier[j]))
		{
			printf("check\n");
			tmp2 = tmp->next->next;
			free(tmp->next->content);
			free(tmp->next);
			tmp->next = tmp2;
			break;
		}
		tmp = tmp->next;
	}
	errno = 0;
}

void	echo(char *cmd, int fd)
{
	int i;
	int returnendline;

	i = 4;
	returnendline = 0;
	while (cmd[i] == ' ')
		i++;
	if (cmd[i] == '-' && cmd[i + 1] == 'n' && cmd[i + 2] == ' ')
	{
		i += 3;
		returnendline = 1;
	}
	while (cmd[i] == ' ')
		i++;
	ft_putstr_fd(&cmd[i], fd);
	if (!returnendline)
		ft_putstr_fd("\n", fd);
	errno = 0;
}

void	ft_export(char *cmd, t_info *info, int fd)
{
	t_list *tmp;
	int i;
	int j;

	tmp = info->env;
	i = 6;
	j = 0;
	while (cmd[i] && cmd[i] == ' ')
		i++;
	if (!cmd[i])
	{
		while (tmp)
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(tmp->content, fd);
			ft_putstr_fd("\n", fd);
			tmp = tmp->next;
		}
	}
	while (cmd[i + j] && cmd[i + j] != '=')
	{
		if (!ft_isalnum(cmd[i + j]) && cmd[i + j] != '_')
		{
			ft_putstr_fd("export: '", 2);
			ft_putstr_fd(&cmd[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			errno = 1;
			return ;
		}
		j++;
	}
	ft_lstadd_back(&info->env, ft_lstnew(ft_strdup(&cmd[i])));
	errno = 0;
}

void	ft_exit(char *cmd, t_info *info, t_err_code err_code)
{
	int i;
	int j;
	int error;
	int nbspace;
	long long int ret;

	i = 4;
	j = -1;
	error = 0;
	nbspace = 0;
	ret = errno;
	if (cmd)
	{
		while (cmd[i] && cmd[i] == ' ')
			i++;
		if (!cmd[i])
		{
			clear_cmd_lst(&info->cmd);
			ft_lstclear(&info->env, &ft_memdel);
			ft_putstr_fd("exit\n", 1);
			exit(ret);
		}
		if (cmd[i] == '-')
			j++;
		while (cmd[i + ++j])
			if (cmd[i + j] == ' ' && cmd[i + j + 1] != ' ' && cmd[i + j + 1])
			{
				nbspace++;
				while (cmd[i + j] == ' ')
					j++;
			}
		if (nbspace)
		{
			errno = 1;
			ft_putstr_fd("exit: too many arguments\n", 2);
			return ;
		}
		error = 0;
		ret = ft_atoll(&cmd[i], &error) % 256;
		ft_putstr_fd("exit\n", 1);
		if (error)
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(&cmd[i], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			ret = 2;
		}
	}
	if (err_code == 1)
		{
			ft_putstr_fd("exit\n", 1);
			ft_putstr_fd("Error : Malloc failed\n", 2);
			ret = 1;
		}
	if (err_code == 2)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("Error : fork failed\n", 2);
		ret = 1;
	}
	if (err_code == 3)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("Error : open failed\n", 2);
		ret = 1;
	}
	clear_cmd_lst(&info->cmd);
	ft_lstclear(&info->env, &ft_memdel);
	exit(ret);
}