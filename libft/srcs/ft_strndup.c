#include "libft.h"

char	*ft_strndup(const char *s, int len)
{
	int		i;
	char	*dst;

	i = 0;
	dst = malloc(sizeof(char) * (len + 1));
	if (!dst)
		return (0);
	while (i < len)
	{
		dst[i] = s[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}