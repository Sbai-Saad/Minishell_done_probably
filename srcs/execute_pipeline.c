/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 05:59:53 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/21 06:00:05 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_child_fail(int saved_errno, t_shell *shell)
{
	int	status;

	if (saved_errno == ENOENT)
		status = 127;
	else if (saved_errno == EACCES || saved_errno == EISDIR
		|| saved_errno == ENOTDIR || saved_errno == ENOEXEC)
		status = 126;
	else
		status = 126;
	child_exec_fail(shell, status);
}

char	*resolve_path_or_exit(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	path = find_command_path(cmd->args[0], shell->env_list);
	if (!path)
	{
		ft_error(cmd->args[0]);
		ft_error(": command not found\n");
		child_exec_fail(shell, 127);
	}
	return (path);
}

void	exec_external_or_exit(t_shell *shell, t_cmd *cmd, char *path)
{
	char	**envp;
	int		saved_errno;

	envp = env_to_array(shell->env_list);
	execve(path, cmd->args, envp);
	saved_errno = errno;
	ft_perror(cmd->args[0]);
	free(path);
	free_env_array(envp);
	exec_child_fail(saved_errno, shell);
}

void	exec_child_pipeline(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	reset_signals();
	if (shell->stdin_copy != -1)
	{
		close(shell->stdin_copy);
		shell->stdin_copy = -1;
	}
	if (shell->stdout_copy != -1)
	{
		close(shell->stdout_copy);
		shell->stdout_copy = -1;
	}
	if (handle_redirections(cmd))
		child_exec_fail(shell, 1);
	if (!cmd->args || !cmd->args[0])
		child_exec_fail(shell, 0);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(shell, cmd));
	path = resolve_path_or_exit(shell, cmd);
	exec_external_or_exit(shell, cmd, path);
}

int	open_next_pipe(int has_next, int pipefd[2], int prev_fd)
{
	if (has_next)
	{
		if (pipe(pipefd) < 0)
		{
			ft_perror("pipe");
			if (prev_fd != -1)
				close(prev_fd);
			return (1);
		}
	}
	return (0);
}
