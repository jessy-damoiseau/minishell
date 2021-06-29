#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct	s_dlist
{
	void			*content;
	struct s_dlist	*next;
	struct s_dlist	*prev;
}				t_dlist;


t_dlist	*ft_dlstnew(void *content)
{
	t_dlist	*l_init;

	l_init	= malloc(sizeof(t_dlist));
	if (!l_init)
		return (0);
	else
	{
		l_init->content = content;
		l_init->next = NULL;
		l_init->prev = NULL;
	}
	return (l_init);
}

void	ft_dlstadd_back(t_dlist **alst, t_dlist *new)
{
	t_dlist	*iter;

	if (!*alst)
	{
		*alst = new;
		return ;
	}
	iter = *alst;
	while (iter->next)
		iter = iter->next;
	iter->next = new;
	new->prev = iter;
}

void	ft_dlstadd_front(t_dlist **alst, t_dlist *new)
{
	new->next = *alst;
	*alst = new;
}

void	ft_dlstclear(t_dlist **lst, void (*del)(void *))
{
	t_dlist	*tmp;

	tmp = NULL;
	if (*lst)
	{
		while (*lst)
		{
			tmp = *lst;
			*lst = (*lst)->next;
			del(tmp->content);
			free(tmp);
		}
	}
	*lst = NULL;
}

void	ft_dlstdelone(t_dlist *lst, void (*del)(void *))
{
	t_dlist	*tmp;

	if (lst)
	{
		tmp = lst;
		lst = lst->next;
		lst->prev = tmp->prev;
		free(tmp);
	}
}

int	ft_dlstsize(t_dlist *lst)
{
	int		l;
	t_dlist	*iter;

	iter = lst;
	l = 0;
	while(iter)
	{
		l++;
		iter = iter->next;
	}
	return (l);
}