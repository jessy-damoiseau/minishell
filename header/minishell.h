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
# include <errno.h>
# include <sys/wait.h>
# include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

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
char	*strpthjoin(char *s1, char *s2);
long long int	ft_atoll(char *str, int *error);

/* TOKENIZE */
t_token_type	ft_define_token(char charset);
void	ft_create_token(char *s, t_info *info);
t_token	*ft_malloc_token(char *input, t_token_type type, int i, int j);

/* PROMPT */
void    ft_prompt(t_info *t_info);
void    ft_sighandler(int signum);

/* EXIT */
void			clear_token(t_dlist *lst, t_token *token);
void			clear_cmd_lst(t_dlist **lst);
void			ft_create_token(char *s, t_info *info);
t_token			*ft_malloc_token(char *input, t_token_type type, int i, int j);
void			ft_prompt(t_info *info);

/* EXIT */
void	ft_clear_token(t_list **lst, void (*del)(void *));
void	free_dbl(char **str);

/* GBCOLLECTOR */
t_gbc	*newgbc(int type, int fd, void *str);
void	gbcclear_one(t_gbc **alst, int maillon, int size);
void	gbcclear(t_gbc **alst);
void	clearmaillon(t_gbc *lst);
void	lstaddback_gbc(t_gbc **alst, t_gbc *new);

/* EXEC_COMMAND */
void	exec_command(t_info *info);

void			ft_create_token(char *s, t_info *info);
t_token			*ft_malloc_token(char *input, t_token_type type, int i, int j);
void			ft_prompt(t_info *info);

/* EXIT */
void	ft_clear_token(t_list **lst, void (*del)(void *));
void    clear_cmd_node(t_dlist **lst, t_info *info);
void	free_dbl(char **str);

/* GBCOLLECTOR */
t_gbc	*newgbc(int type, int fd, void *str);
void	gbcclear_one(t_gbc **alst, int maillon, int size);
void	gbcclear(t_gbc **alst);
void	clearmaillon(t_gbc *lst);
void	lstaddback_gbc(t_gbc **alst, t_gbc *new);

/* EXEC_COMMAND */
void	exec_command(t_info *info);
void	check_end(t_dlist **alst);
int	check_exist(char *str);
int	check_builtins(t_info *info, t_dlist *mcmd);
int check_exec(t_info *info, t_dlist *mcmd);

/* BUILTINS */
void	pwd(t_info *info, char *cmd, int fd);
void	cd(char *cmd);
void    env(char *cmd, t_info *info, int fd);
void	unset(char *cmd, t_info *info);
void	echo(char *cmd, int fd);
void	ft_export(char *cmd, t_info *info, int fd);
void	ft_exit(char *cmd, t_info *info, t_err_code err_code);

/* QUOTE */
void    parse_quote(t_info *info);
int    find_token_type(t_token_type type, t_token *token);

/* PARSE */
void	parse_token(t_info *info);

/* PIPELINE */
void    create_pipeline(t_info *info, int pipe);

/* PARSE ENV */
void	parse_env(t_info *info);

/* EXPAND ENV */
void	expand_env(t_info *info);

/* REDIRECT */
void	redirection(t_info *info);

void	exec_pipeline(t_dlist *list, t_info *info);

#endif