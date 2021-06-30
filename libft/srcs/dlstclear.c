#include "libft.h"

void	dlstclear(t_dlist **lst, void (*del)(void *))
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