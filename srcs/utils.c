#include "minishell.h"

int	ft_go_malloc(char **str, int len)
{
	(*str) = malloc(len);
	if (!str)
		return (1);
	return (0);
}

void	error_dbl(char **ret, int size)
{
	int i;

	i = 0;
	while (i < size)
		free(ret[i++]);
	free(ret);
	ret = 0;
}

char	**dbl_chardup(char **envp)
{
	char	**dest;
	int		i;
	int		j;

	i = 0;
	dest = 0;
	while (envp[i])
		i++;
	dest = malloc(sizeof(char *) * (i + 1));
	if (!dest)
		return (0);
	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (envp[i][j])
			j++;
		if (ft_go_malloc(&dest[i], (sizeof(char) * (j + 1))))
			return (0);
		j = -1;
		while (envp[i][++j])
			dest[i][j] = envp[i][j];
		dest[i][j] = '\0';
	}
	dest[i] = 0;
	return (dest);
}

int	ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	if (s1[i] == s2[i])
		return (0);
	return (1);
}

char	*strpthjoin(char *s1, char *s2)
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

long long int	ft_atoll(char *str, int *error)
{
	int				i;
	int				s;
	long long int	r;

	i = 0;
	s = 1;
	r = 0;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
		i++;
	if (str[i] == '-')
	{
		s *= -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] && (str[i] >= 48 && str[i] <= 57))
	{
		r = r * 10 + str[i] - 48;
		i++;
	}
	if (r < 0 || str[i])
		*error = 1;
	return (r * s);
}