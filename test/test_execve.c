#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int		count_words(char const *s, char c)
{
	int		i;
	int		nb_words;

	nb_words = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			nb_words++;
		i++;
	}
	return (nb_words);
}

int		ft_len(char const *s, char c)
{
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (s[i] != c && s[i] != '\0')
	{
		i++;
		len++;
	}
	return (len);
}

void	*leak(char **dest, int nb_words)
{
	int	i;

	i = 0;
	while (i < nb_words)
	{
		free(dest[i]);
		i++;
	}
	free(dest);
	return (0);
}

char	**fill(char const *s, int nb_words, char c, char **dest)
{
	int		i;
	int		j;
	int		len;

	i = -1;
	while (++i < nb_words)
	{
		while (*s == c)
			s++;
		len = ft_len(s, c);
		if (!(dest[i] = (char *)malloc(sizeof(char) * (len + 1))))
			return (leak(dest, i));
		j = 0;
		while (j < len)
			dest[i][j++] = *s++;
		dest[i][j] = '\0';
	}
	dest[i] = 0;
	return (dest);
}

char	**ft_split(char	const *s, char c)
{
	char	**dest;
	int		nb_words;

	if (!s)
		return (0);
	nb_words = count_words(s, c);
	if (!(dest = (char **)malloc(sizeof(char *) * (nb_words + 1))))
		return (0);
	dest = fill(s, nb_words, c, dest);
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	j;
	size_t	i;
	char	*c;

	if (!s1 || !s2)
		return (0);
	i = strlen(s1);
	j = strlen(s2);
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

int main(int ac, char **av, char **ev)
{
    char **path;
	char *env_path;
	char *check_path;
    (void)ac;
    int t;
	int i = 0;

	env_path = getenv("PATH");
	path = ft_split(env_path, ':');
	while (path[i])
	{
		check_path = ft_strjoin(path[i], av[1]);
    	execve(check_path, &av[1], ev);
		i++;
	}
    return (0);
}