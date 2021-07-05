#include "libft.h"

t_dlist	*dlstnew(void *content)
{
	t_dlist	*l_init;

	if (!content)
		return (0);
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