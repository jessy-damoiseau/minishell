#ifndef TEST_H
# define TEST_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

int		ft_strlen(char const *s);
char	**ft_split(char	const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
int	    go_malloc(char **str, int len);
int	check_command(char *line, char **env);
#endif