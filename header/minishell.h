#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "libft.h"
# include "struct.h"

# define STR 0
# define FD 1
# define LST 2
# define GBC 3

/* UTILS */
int	ft_go_malloc(char **str, int len);

/* TOKENIZE */
t_token_type	ft_define_token(char charset);
void	ft_create_token(char *s, t_info *info);
t_token	*ft_malloc_token(char *input, t_token_type type, int i, int j);

/* PROMPT */
void    ft_prompt(t_info *info);
void    ft_sighandler(int signum);

/* EXIT */
void	ft_clear_token(t_list **lst, void (*del)(void *));
void    ft_exit(t_info *info, t_err_code err_code);

#endif