#include "minishell.h"

static void    ft_get_env(t_info *info, char **envp)
{
	int     i;
	char    *str;
	t_list  *new;

	i = 0;
	info->env = NULL;
	info->cmd = NULL;
	while(envp[i])
	{
		str = ft_strdup(envp[i]);
		new = ft_lstnew(str);
		ft_lstadd_back(&info->env, new);
		i++;
	}
}

char **duplst(t_list *env ,t_info *info)
{
	char **ret;
	int i;

	i = 0;
	ret = malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	if (!ret)
		ft_exit(0, info, err_malloc);
	while(env)
	{
		ret[i++] = ft_strdup(env->content);
		env = env->next;
	}
	ret[i] = 0;
	return (ret);
}

void	init_struct(t_info *info)
{
	info->pwd = 0;
	info->gbc = 0;
	info->path = 0;
	info->cmd = 0;
	info->nbpipe = 0;
	info->evrm = duplst(info->env, info);
}

void change_shlv(t_list **env)
{
	t_list *tmp;
	char *str;
	int	i;

	tmp = *env;
	while (tmp && ft_strncmp(tmp->content, "SHLVL", 5))
		tmp = tmp->next;
	str = tmp->content;
	i = ft_atoi(&str[6]) + 1;
	free(tmp->content);
	tmp->content = ft_strjoin("SHLVL=", ft_itoa(i));
}

int main(int ac, char **av, char **envp)
{
	t_info  info;

	(void)ac;
	(void)av;
	printf("prog lancer\n");
	ft_get_env(&info, envp);
	change_shlv(&info.env);
	init_struct(&info);
	signal(SIGINT, ft_sighandler);
	signal(SIGQUIT, SIG_IGN);
	ft_prompt(&info);
	ft_exit(0, &info, no_err);
	return (0);
}