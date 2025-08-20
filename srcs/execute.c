/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:19:06 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:19:07 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exit_not_found_with_path_unset(t_cmd *cmd)
{
	if (ft_strcmp(cmd->args[0], ".") == 0
		|| ft_strcmp(cmd->args[0], "..") == 0)
	{
		ft_error(cmd->args[0]);
		ft_error(": Is a directory\n");
		exit(126);
	}
	write(2, "bash: line 1: ", 15);
	if (cmd->args[0] && cmd->args[0][0])
		write(2, cmd->args[0], ft_strlen(cmd->args[0]));
	else
		write(2, "", 0);
	write(2, ": No such file or directory\n", 28);
	exit(127);
}

char	*resolve_path_for_child(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	*path_var;
	int		path_unset;

	path = find_command_path(cmd->args[0], shell->env_list);
	if (path)
		return (path);
	path_var = get_env_value(shell->env_list, "PATH");
	path_unset = (!path_var || path_var[0] == '\0');
	if (path_unset)
	{
		exit_not_found_with_path_unset(cmd);
	}
	exit_command_not_found(cmd);
	return (NULL);
}

void	ensure_not_directory(char *path, t_cmd *cmd)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_error(cmd->args[0]);
		ft_error(": Is a directory\n");
		free(path);
		exit(126);
	}
}

void	child_try_execve(t_shell *shell, t_cmd *cmd, char *path)
{
	char		**envp;
	int			saved_errno;
	char		*sh_argv[3];

	envp = env_to_array(shell->env_list);
	execve(path, cmd->args, envp);
	saved_errno = errno;
	if (saved_errno == ENOEXEC)
	{
		sh_argv[0] = (char *)"bash";
		sh_argv[1] = path;
		sh_argv[2] = NULL;
		execve("/bin/bash", sh_argv, envp);
		saved_errno = errno;
	}
	ft_perror(cmd->args[0]);
	free(path);
	free_env_array(envp);
	if (saved_errno == ENOENT)
		exit(127);
	if (saved_errno == EACCES || saved_errno == EISDIR
		|| saved_errno == ENOTDIR || saved_errno == ENOEXEC)
		exit(126);
	exit(126);
}

void	execute_child_process(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	reset_signals();
	if (handle_redirections(cmd))
		exit(1);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	path = resolve_path_for_child(shell, cmd);
	ensure_not_directory(path, cmd);
	child_try_execve(shell, cmd, path);
}
