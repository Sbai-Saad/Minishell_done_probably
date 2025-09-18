/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:18:57 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:18:57 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_commands(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = shell->cmds;
	if (!cmd)
		return (0);
	handle_heredocs(cmd, shell);
	if (g_signal_received)
	{
		shell->exit_status = 130;
		g_signal_received = 0;
		return (shell->exit_status);
	}
	if (!cmd->next)
		return (execute_single_command(shell, cmd));
	else
		return (execute_pipeline(shell, cmd));
}

static int	start_single_child(t_shell *shell, t_cmd *cmd,
		struct termios *saved_attr, pid_t *out_pid)
{
	pid_t	pid;

	ignore_signals();
	tcgetattr(STDOUT_FILENO, saved_attr);
	pid = fork();
	if (handle_fork_error(pid))
	{
		setup_signals();
		return (1);
	}
	if (pid == 0)
		execute_child_process(shell, cmd);
	*out_pid = pid;
	return (0);
}

static int	wait_and_finalize_single(t_shell *shell, pid_t pid,
		struct termios *saved_attr)
{
	int	status;

	while (waitpid(pid, &status, 0) < 0)
	{
		if (errno == EINTR)
			continue ;
		ft_perror("waitpid");
		status = 1 << 8;
		break ;
	}
	tcsetattr(STDOUT_FILENO, 0, saved_attr);
	setup_signals();
	parent_post_wait_status(shell, status);
	return (shell->exit_status);
}

int	execute_single_command(t_shell *shell, t_cmd *cmd)
{
	pid_t			pid;
	struct termios	attr;

	if (cmd->args && is_builtin(cmd->args[0]))
		return (run_builtin_in_parent(shell, cmd));
	if (start_single_child(shell, cmd, &attr, &pid) != 0)
		return (1);
	return (wait_and_finalize_single(shell, pid, &attr));
}
