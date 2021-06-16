#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
# define STR 0
# define FD 1
# define LST 2
# define GBC 3

typedef struct  s_gbcollector
{
    int type;
    int fd;
    void *str;
    struct s_gbcollector *next;
}               t_gbc;

typedef struct s_lst
{
	void *content;
	struct s_lst *next;
}				t_lst;

void	gbcclear(t_gbc **alst);

int	go_malloc(void **ptr, int len)
{
	(*ptr) = malloc(len);
	if (!ptr)
		return (1);
	return (0);
}

void lstaddback(t_gbc **alst, t_gbc *new)
{
    t_gbc	*tmp;

	if (!*alst)
	{
		*alst = new;
		return ;
	}
	tmp = *alst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
t_gbc   *newgbc(int type, int fd, void *str)
{
    t_gbc *new;
    if (go_malloc((void *)&new, sizeof(t_gbc)))
        return (0);
    new->type = type;
    new->fd = fd;
    new->str = str;
    new->next = 0;
    return (new);
}

void	lstclear(t_lst **lst)
{
	t_lst	*tmp;

	tmp = 0;
	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		free(tmp);
	}
	*lst = 0;
}

void	clearmaillon(t_gbc *lst)
{
	if (lst->type == FD && lst->fd > 0)
		close(lst->fd);
	else if (lst->type == STR)
		free(lst->str);
	else if (lst->type == LST)
		lstclear((t_lst **)&lst->str);
	else
		gbcclear((t_gbc **)&lst->str);
	free(lst);
}

void	gbcclear(t_gbc **alst)
{
	t_gbc *tmp;

	while (*alst)
	{
		tmp = *alst;
		*alst = (*alst)->next;
		clearmaillon(tmp);
	}
	*alst = 0;
}

void gbcclear_one(t_gbc **alst, int maillon, int size)
{
	t_gbc *tmp;
	t_gbc *tmp2;
	int i;

	tmp = *alst;
	tmp2 = 0;
	i = 0;
	while (i < maillon)
	{
		if (i == maillon - 1)
			tmp2 = tmp;
		tmp = tmp->next;
		i++;
	}
	if (maillon != size)
	{
		if (!tmp2)
			(*alst) = (*alst)->next;
		else
			tmp2->next = tmp->next;
	}
	else 
		tmp2->next = 0;
	clearmaillon(tmp);
}

int main(int ac, char **av)
{
    t_gbc *lst = 0;
    t_gbc *tmp;
    (void)ac;
    lstaddback(&lst, newgbc(FD, open(av[1], O_RDONLY), 0));
    lstaddback(&lst, newgbc(STR, -1, strdup(av[2])));
    lstaddback(&lst, newgbc(STR, -1, strdup(av[3])));
    lstaddback(&lst, newgbc(FD, open(av[4], O_RDONLY), 0));
    lstaddback(&lst, newgbc(STR, -1, strdup(av[5])));
    lstaddback(&lst, newgbc(STR, -1, strdup(av[6])));
    lstaddback(&lst, newgbc(STR, -1, strdup(av[7])));
    lstaddback(&lst, newgbc(STR, -1, strdup(av[8])));
    printf("------------\n");
    tmp = lst;
    while (tmp)
    {
        printf("type: |%d|\nfd:   |%d|\nstr:  |%s|\n------------\n", tmp->type, tmp->fd, tmp->str);
        tmp = tmp->next;
    }
    gbcclear_one(&lst, 6, 7);
	//gbcclear(&lst);
	printf("\nclear done\n\n");
	printf("------------\n");
    tmp = lst;
    while (tmp)
    {
        printf("type: |%d|\nfd:   |%d|\nstr:  |%s|\n------------\n", tmp->type, tmp->fd, tmp->str);
        tmp = tmp->next;
    }
	gbcclear(&lst);
    return (0);
}