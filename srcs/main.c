#include "minishell.h"

static void    ft_get_env(t_info *info, char **envp)
{
    int     i;
    char    *str;
    t_list  *new;

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

int main(int ac, char **av, char **envp)
{
    t_info  info;

    (void)ac;
    (void)av;
    ft_get_env(&info, envp);
    ft_prompt(&info);
    ft_exit(&info, no_err);
    return (0);
}