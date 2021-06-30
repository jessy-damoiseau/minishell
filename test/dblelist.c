#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

typedef struct	s_dlist
{
	void			*content;
	struct s_dlist	*next;
	struct s_dlist	*prev;
}				t_dlist;

void	ft_del(void *content)
{
	free(content);
	content = NULL;
}

size_t	ft_strlen(const char *s)
{
	if (*s == '\0')
		return (0);
	return (ft_strlen(s + 1) + 1);
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (i <= n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

t_dlist	*ft_dlist_end(t_dlist *list)
{
	t_dlist	*iter;

	iter = list;
	while (iter->next)
	{
		iter = iter->next;
	}
	return (iter);
}

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
	(*alst)->prev = new;
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
		tmp->prev->next = lst;
		del(tmp->content);
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

t_dlist	*ft_dlist_find_node(t_dlist *list, void *content)
{
	t_dlist *tmp;
	
	tmp = list;
	while (tmp)
	{
		if (!ft_memcmp(tmp->content, content, ft_strlen((char *)content)))
			return (tmp);
		tmp = tmp->next;
	}
	return (0);
}

void	ft_dlist_insert_node(t_dlist **alst, t_dlist *pos, t_dlist *new)
{
	t_dlist	*iter;

	if (!*alst)
	{
		*alst = new;
		return ;
	}
	iter = *alst;
	if (pos)
	{
		while (iter != pos)
			iter = iter->next;
		if (!pos->next)
		{
			ft_dlstadd_back(alst, new);
			return ;
		}
		new->next = iter->next;
		iter->next->prev = new;
		iter->next = new;
		new->prev = iter;
	}
}

int main(void)
{
	t_dlist	*dlist;
	t_dlist	*new;

	dlist = NULL;
	new = ft_dlstnew(strdup("test2"));
	ft_dlstadd_back(&dlist, new);
	new = ft_dlstnew(strdup("test1"));
	ft_dlstadd_front(&dlist, new);
	new = ft_dlstnew(strdup("test3"));
	ft_dlstadd_back(&dlist, new);
	t_dlist	*tmp;
	tmp = dlist;
	printf("A l'endroit\n");
	while (tmp)
	{
		printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
	t_dlist	*test;
	test =  ft_dlist_find_node(dlist, "test3");
	if (test)
		printf("test finder = %s\n", test->content);
	else
		printf("node not found\n");
	tmp = ft_dlist_end(dlist);
	printf("A l'envers\n");
	while (tmp)
	{
		printf("%s\n", tmp->content);
		tmp = tmp->prev;
	}
	printf("test avec 1 maillon de plus\n");
	new = ft_dlstnew(strdup("test2bis"));
	ft_dlist_insert_node(&dlist, ft_dlist_find_node(dlist, "test1"), new);
	tmp = dlist;
	printf("A l'endroit\n");
	while (tmp)
	{
		printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
	printf("test avec 1 maillon de moins\n");
	ft_dlstdelone(ft_dlist_find_node(dlist, "test2bis"), &ft_del);
	tmp = dlist;
	printf("A l'endroit\n");
	while (tmp)
	{
		printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
	ft_dlstclear(&dlist, &ft_del);
	return (0);
}