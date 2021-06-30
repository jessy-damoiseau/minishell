#include "libft.h"

t_dlist *dlst_end(t_dlist *list)
{
	t_dlist *iter;

	iter = list;
	while (iter->next)
	{
		iter = iter->next;
	}
	return (iter);
}