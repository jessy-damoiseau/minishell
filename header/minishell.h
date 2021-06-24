#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "struct.h"
# include <errno.h>

# define STR 0
# define FD 1
# define LST 2
# define GBC 3
# define TOKEN 4

/* UTILS */
int		ft_go_malloc(char **str, int len);
char	**dbl_chardup(char **envp);
void	error_dbl(char **ret, int size);
int		ft_strcmp(char *s1, char *s2);

/* TOKENIZE */
t_token_type	ft_define_token(char charset);
void	        ft_create_token(char *s, t_info *info);
t_token	        *ft_malloc_token(char *input, t_token_type type, int i, int j);
void            ft_prompt(t_info *info);

/* EXIT */
void	ft_clear_token(t_list **lst, void (*del)(void *));
void    ft_exit(t_info *info, t_err_code err_code);

/* GBCOLLECTOR */
t_gbc   *newgbc(int type, int fd, void *str);
void	gbcclear_one(t_gbc **alst, int maillon, int size);
void	gbcclear(t_gbc **alst);
void	clearmaillon(t_gbc *lst);
void	lstaddback_gbc(t_gbc **alst, t_gbc *new);

/* EXEC_COMMAND */
void	exec_command(t_info *info);

#endif