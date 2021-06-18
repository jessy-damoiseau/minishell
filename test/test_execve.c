#include "utils/test.h"

int	check_command(char *line, char **env)
{
    char **path;
	char *check_path;
	char *env_path;
	char **command;
	int i = 0;
	pid_t pid;
	struct stat sb;

	command = ft_split(line, ' ');
	env_path = getenv("PATH");
	path = ft_split(env_path, ':');
	while (path[i])
	{
		check_path = ft_strjoin(path[i], command[0]);
		if (!stat(check_path, &sb))
		{
			pid = fork();
			if (!pid)
				execve(check_path, command, env);
			else
				wait(0);
			return (0);
		}
		free(check_path);
		i++;
	}
	return (1);
}