/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:18:53 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:18:53 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_exec_file(const char *path)
{
	struct stat	st;

	if (access(path, X_OK) != 0)
		return (0);
	if (stat(path, &st) != 0)
		return (0);
	return (S_ISREG(st.st_mode));
}

char	*search_in_paths(char *cmd, char **paths)
{
	int		i;
	char	*tmp;
	char	*full_path;

	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (is_exec_file(full_path))
			return (full_path);
		free(full_path);
	}
	return (NULL);
}

char	*find_command_path(char *cmd, t_env *env)
{
	char	**paths;
	char	*path_var;
	char	*full_path;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = get_env_value(env, "PATH");
	if (!path_var || path_var[0] == '\0')
	{
		if (is_exec_file(cmd))
			return (ft_strdup(cmd));
		return (NULL);
	}
	paths = ft_split(path_var, ':');
	full_path = search_in_paths(cmd, paths);
	ft_free_arr(paths);
	return (full_path);
}

int	handle_fork_error(pid_t pid)
{
	if (pid == -1)
	{
		ft_perror("fork");
		return (1);
	}
	return (0);
}

void	set_signal_flags(int status, int *any_sigint, int *any_sigquit)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT && any_sigint)
			*any_sigint = 1;
		if (WTERMSIG(status) == SIGQUIT && any_sigquit)
			*any_sigquit = 1;
	}
}
