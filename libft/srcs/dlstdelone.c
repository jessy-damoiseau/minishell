#include "libft.h"

void	dlstdelone(t_dlist *lst, void (*del)(void *))
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