#include "minishell.h"

static void    ft_get_env(t_info *info, char **envp)
{
	int     i;
	char    *str;
	t_list  *new;

<<<<<<< HEAD
    i = 0;
    info->env = NULL;
    info->cmd = NULL; // sauf si on cree fonction init ailleurs autant le faire ici
    while(envp[i])
    {
        str = ft_strdup(envp[i]);
        new = ft_lstnew(str);
        ft_lstadd_back(&info->env, new);
        i++;
    }
=======
	i = 0;
	info->env = NULL;
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
>>>>>>> execve
}

int main(int ac, char **av, char **envp)
{
	t_info  info;

<<<<<<< HEAD
    (void)ac;
    (void)av;
	// signal(SIGINT, ft_sighandler); // gestion ctrl C - ne fonctionne pas encore
    ft_get_env(&info, envp);
    ft_prompt(&info);
    ft_exit(&info, no_err);
    return (0);
=======
	(void)ac;
	(void)av;
	init_struct(&info, envp);
	ft_get_env(&info, envp);
	ft_prompt(&info);
	ft_exit(&info, no_err);
	return (0);
>>>>>>> execve
}