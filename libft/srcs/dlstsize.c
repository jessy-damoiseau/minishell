#include "libft.h"

int	dlstsize(t_dlist *lst)
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