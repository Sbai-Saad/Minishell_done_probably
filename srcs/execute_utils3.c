/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:19:02 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:19:02 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	run_builtin_in_parent(t_shell *shell, t_cmd *cmd)
{
	int	ret;

	ignore_signals();
	shell->stdin_copy = dup(STDIN_FILENO);
	shell->stdout_copy = dup(STDOUT_FILENO);
	if (parent_dup_stdio(shell))
		return (1);
	if (handle_redirections(cmd))
	{
		restore_redirections(shell->stdin_copy, shell->stdout_copy);
		shell->exit_status = 1;
		setup_signals();
		return (shell->exit_status);
	}
	ret = execute_builtin(shell, cmd);
	restore_redirections(shell->stdin_copy, shell->stdout_copy);
	setup_signals();
	return (ret);
}

void	parent_post_wait_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (isatty(STDIN_FILENO))
		{
			if (WTERMSIG(status) == SIGINT)
				write(1, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				write(1, "Quit\n", 6);
		}
	}
}

int	parent_dup_stdio(t_shell *shell)
{
	if (shell->stdin_copy == -1 || shell->stdout_copy == -1)
	{
		if (shell->stdin_copy != -1)
			close(shell->stdin_copy);
		if (shell->stdout_copy != -1)
			close(shell->stdout_copy);
		ft_perror("dup");
		setup_signals();
		return (1);
	}
	return (0);
}
