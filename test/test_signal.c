#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void    ft_sighandler(int signum);

/* gestion du ctrl C */
void    ft_sighandler(int signum)
{
    if (signum == SIGINT)
        printf("Cmd ctrl C = %d\n", signum);
    if (signum == SIGQUIT) // pas le bon signal pour ctrl D => EOF
        printf("Cmd ctrl D = %d\n", signum);
    return ;
}

int main(void)
{
    signal(SIGINT, ft_sighandler);
    signal(SIGQUIT, ft_sighandler);
    while (1)
    {
        printf("prompt\n");
        sleep(1);
    }

    return (0);
}