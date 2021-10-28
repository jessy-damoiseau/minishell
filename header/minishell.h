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

/* BUILTINS NORMER */
int		error_code(t_err_code err_code);
int		exit2bis(char *cmd, int *i, int j);
int		exit2(char *cmd, int *i, t_info *info, int j);
int		exit3(char *cmd, int i, t_info *info);
void	ft_exit(char *cmd, t_info *info, t_err_code err_code);
void	pwd(t_info *info, char *cmd, int fd);
void	oldpwd(t_info *info);
int		cd1(char *cmd, int i, int *j);
void	cd2(char *cmd, int j, char *str);
void	cd(char *cmd, t_info *info);
void	env2(char *cmd);
void	env(char *cmd, t_info *info, int fd);
void	unset2(char *cmd, int i, int j, t_info *info);
void	unset(char *cmd, t_info *info);
void	echo(char *cmd, int fd);
void	export1(t_info	*info, int fd);
void	export2(char *cmd, int i, t_info *info);
void	export3(int *i, int *j, int *add, char *cmd);
void	export4(t_list	*tmp, char *cmd, int i, int j);
void	export5(t_list	*tmp, char *cmd, int i);
void	ft_export(char *cmd, t_info *info, int fd);
/* END BUILTINS NORMER */
















/* UTILS */
int				ft_go_malloc(char **str, int len);
char			**dbl_chardup(char **envp);
void			error_dbl(char **ret, int size);
int				ft_strcmp(char *s1, char *s2);
char			*strpthjoin(char *s1, char *s2);
long long int	ft_atoll(char *str, int *error);
char			*supp_add(char *str, t_info *info);
void			tmplstclear(t_dlist **lst);
void			init_var(t_info *info);
size_t          ft_strlen_utils(const char *s, char stop);

/* TOKENIZE */
t_token_type	ft_define_token(char charset);
void			ft_create_token(char *s, t_info *info);
t_token			*ft_malloc_token(char *input, t_token_type type, int i, int j);

/* PROMPT */
void			ft_prompt(t_info *t_info);
void			ft_sighandler(int signum);
char			*get_prompt(t_info *info);

/* EXIT */
void			clear_token(t_dlist *lst, t_token *token);
void			clear_cmd_lst(t_dlist **lst);
void			ft_create_token(char *s, t_info *info);
t_token			*ft_malloc_token(char *input, t_token_type type, int i, int j);
void			ft_prompt(t_info *info);



/* GBCOLLECTOR */
t_gbc			*newgbc(int type, int fd, void *str);
void			gbcclear_one(t_gbc **alst, int maillon, int size);
void			gbcclear(t_gbc **alst);
void			clearmaillon(t_gbc *lst);
void			lstaddback_gbc(t_gbc **alst, t_gbc *new);

/* EXEC_COMMAND */
void			exec_command(t_info *info);

void			ft_create_token(char *s, t_info *info);
t_token			*ft_malloc_token(char *input, t_token_type type, int i, int j);
void			ft_prompt(t_info *info);

/* EXIT */
void			ft_clear_token(t_list **lst, void (*del)(void *));
void			clear_cmd_node(t_dlist **lst, t_info *info);
void			free_dbl(char **str);

/* GBCOLLECTOR */
t_gbc			*newgbc(int type, int fd, void *str);
void			gbcclear_one(t_gbc **alst, int maillon, int size);
void			gbcclear(t_gbc **alst);
void			clearmaillon(t_gbc *lst);
void			lstaddback_gbc(t_gbc **alst, t_gbc *new);

/* EXEC_COMMAND */
void			exec_command(t_info *info);
void			check_end(t_dlist **alst);
int				check_exist(char *str);
int				check_builtins(t_info *info, t_dlist *mcmd);
int				check_exec(t_info *info, t_dlist *mcmd);

/* BUILTINS */
void			pwd(t_info *info, char *cmd, int fd);
void			cd(char *cmd, t_info *info);
void			env(char *cmd, t_info *info, int fd);
void			unset(char *cmd, t_info *info);
void			echo(char *cmd, int fd);
void			ft_export(char *cmd, t_info *info, int fd);
void			ft_exit(char *cmd, t_info *info, t_err_code err_code);

/* QUOTE */
void			parse_quote(t_info *info);
int				find_token_type(t_token_type type, t_token *token);
t_dlist			*clean_sublst(t_dlist *begin, t_dlist *end, t_info *info);
void			clear_buffer(char *buf, char *tmp, t_info *info);

/* PARSE */
void			parse_token(t_info *info);

/* PIPELINE */
void			create_pipeline(t_info *info);

/* PARSE ENV */
void			parse_env(t_info *info);

/* EXPAND ENV */
void			expand_env(t_info *info);
void			replace_node_value(char *evar, t_dlist **iter);
int				find_env_var(t_dlist **iter, t_info *info);
int				find_errno_type(t_dlist *lst);
void			replace_errno(char *str, t_dlist **lst, t_info *info);

/* REDIRECT */
void			redirection(t_info *info);

void			exec_pipeline(t_dlist *list, t_info *info);

#endif