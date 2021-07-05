#include "libft.h"

void	ft_memdel(void **content)
{
	free(*content);
	*content = NULL;
}