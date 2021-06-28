#include "minishell.h"

static void    ft_get_env(t_info *info, char **envp)
{
    int     i;
    char    *str;
    t_list  *new;

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
}

int main(int ac, char **av, char **envp)
{
    t_info  info;

    (void)ac;
    (void)av;
	// signal(SIGINT, ft_sighandler); // gestion ctrl C - ne fonctionne pas encore
    ft_get_env(&info, envp);
    ft_prompt(&info);
    ft_exit(&info, no_err);
    return (0);
}