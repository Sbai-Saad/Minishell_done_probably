/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:18:44 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:18:44 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_pipeline(t_shell *shell, t_cmd *cmd)
{
	int		prev_fd;
	pid_t	pid;

	pid = 0;
	prev_fd = -1;
	shell->stdin_copy = dup(STDIN_FILENO);
	shell->stdout_copy = dup(STDOUT_FILENO);
	ignore_signals();
	if (cmd && pipeline_spawn_all(shell, cmd, &prev_fd, &pid))
	{
		restore_redirections(shell->stdin_copy, shell->stdout_copy);
		return (1);
	}
	return (pipeline_finalize(shell, pid));
}

int	pipeline_finalize(t_shell *shell, pid_t last_pid)
{
	int	any_sigint;
	int	any_sigquit;

	restore_redirections(shell->stdin_copy, shell->stdout_copy);
	any_sigint = 0;
	any_sigquit = 0;
	shell->exit_status = wait_all_pipes(last_pid, &any_sigint, &any_sigquit);
	post_wait_prints(any_sigint, any_sigquit);
	setup_signals();
	return (shell->exit_status);
}

int	pipeline_spawn_all(t_shell *shell, t_cmd *cmd, int *prev_fd,
							pid_t *last_pid)
{
	while (cmd)
	{
		if (spawn_one(shell, cmd, prev_fd, last_pid))
			return (1);
		cmd = cmd->next;
	}
	return (0);
}

void	post_wait_prints(int any_sigint, int any_sigquit)
{
	if (isatty(STDIN_FILENO))
	{
		if (any_sigint)
			write(1, "\n", 1);
		if (any_sigquit)
			write(1, "Quit\n", 6);
	}
}

int	spawn_one(t_shell *shell, t_cmd *cmd, int *prev_fd, pid_t *out_pid)
{
	int	pipefd[2];
	int	has_next;

	has_next = (cmd->next != NULL);
	if (open_next_pipe(has_next, pipefd, *prev_fd))
		return (1);
	*out_pid = fork();
	if (handle_fork_error(*out_pid))
		return (fork_failed_cleanup(has_next, pipefd, *prev_fd));
	if (*out_pid == 0)
	{
		child_dup_and_close(*prev_fd, has_next, pipefd);
		exec_child_pipeline(shell, cmd);
	}
	parent_close_and_shift(prev_fd, has_next, pipefd);
	return (0);
}
