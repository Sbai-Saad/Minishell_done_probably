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

int	compute_last_status(int status, int last_status,
							pid_t pid, int last_pid)
{
	if (pid == last_pid)
	{
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	return (last_status);
}

int	wait_loop(int last_pid, int *any_sigint, int *any_sigquit)
{
	int		status;
	int		last_status;
	pid_t	pid;

	last_status = 0;
	pid = 1;
	while (pid > 0)
	{
		pid = wait(&status);
		if (pid <= 0)
			break ;
		set_signal_flags(status, any_sigint, any_sigquit);
		last_status = compute_last_status(status, last_status, pid, last_pid);
	}
	return (last_status);
}

int	wait_all_pipes(int last_pid, int *any_sigint, int *any_sigquit)
{
	int	last_status;

	if (any_sigint)
		*any_sigint = 0;
	if (any_sigquit)
		*any_sigquit = 0;
	last_status = wait_loop(last_pid, any_sigint, any_sigquit);
	return (last_status);
}

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

int	execute_single_command(t_shell *shell, t_cmd *cmd)
{
	pid_t			pid;
	int				status;
	struct termios	attr;

	if (cmd->args && is_builtin(cmd->args[0]))
		return (run_builtin_in_parent(shell, cmd));
	ignore_signals();
	tcgetattr(STDOUT_FILENO, &attr);
	pid = fork();
	if (handle_fork_error(pid))
	{
		setup_signals();
		return (1);
	}
	if (pid == 0)
		execute_child_process(shell, cmd);
	waitpid(pid, &status, 0);
	tcsetattr(STDOUT_FILENO, 0, &attr);
	setup_signals();
	parent_post_wait_status(shell, status);
	return (shell->exit_status);
}
