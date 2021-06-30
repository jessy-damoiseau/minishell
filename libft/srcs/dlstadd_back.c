#include "libft.h"

void	dlstadd_back(t_dlist **alst, t_dlist *new)
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