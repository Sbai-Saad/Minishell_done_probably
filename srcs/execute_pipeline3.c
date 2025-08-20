/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:18:48 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:18:48 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	child_dup_and_close(int prev_fd, int has_next, int pipefd[2])
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (has_next)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

void	parent_close_and_shift(int *prev_fd, int has_next, int pipefd[2])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (has_next)
	{
		*prev_fd = pipefd[0];
		close(pipefd[1]);
	}
}

int	fork_failed_cleanup(int has_next, int pipefd[2], int prev_fd)
{
	if (has_next)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (1);
}

void	child_exec_fail(t_shell *shell, int status)
{
	if (shell->stdout_copy != -1)
		close(shell->stdout_copy);
	if (shell->stdin_copy != -1)
		close(shell->stdin_copy);
	exit(status);
}
