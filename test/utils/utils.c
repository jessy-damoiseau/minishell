#include "test.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	j;
	size_t	i;
	char	*c;

	if (!s1 || !s2)
		return (0);
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	if (!(c = malloc(sizeof(*c) * (i + j + 2))))
		return (0);
	i = 0;
	j = -1;
	while (s1[i])
	{
		c[i] = s1[i];
		i++;
	}
	c[i++] = '/';
	while (s2[++j])
		c[i++] = s2[j];
	c[i] = 0;
	return (c);
}

int	go_malloc(char **str, int len)
{
	(*str) = malloc(len);
	if (!str)
		return (1);
	return (0);
}

int ft_strlen(char const *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}