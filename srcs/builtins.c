#include "minishell.h"

void    pwd(char *str, int fd)
{
    ft_putstr_fd(str, fd);
    if (fd == 1)
        write(fd, "\n", 1);
    errno = 0;
}