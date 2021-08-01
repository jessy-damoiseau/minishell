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

void	init_struct(t_info *info, char **envp)
{
	info->pwd = 0;
	info->gbc = 0;
	info->path = 0;
	info->cmd = 0;
	info->evrm = dbl_chardup(envp);
}

int main(int ac, char **av, char **envp)
{
	t_info  info;

	(void)ac;
	(void)av;
	printf("prog lancer\n");
	init_struct(&info, envp);
	ft_get_env(&info, envp);
	signal(SIGINT, ft_sighandler);
	signal(SIGQUIT, SIG_IGN);
	ft_prompt(&info);
	ft_exit(0, &info, no_err);
	return (0);
}