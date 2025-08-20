/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:20:12 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:20:12 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	setup_prev_pipe(int *prev_pipe)
{
	if (prev_pipe[0] != -1)
	{
		if (dup2(prev_pipe[0], STDIN_FILENO) < 0)
		{
			ft_perror("dup2");
			return (ERROR);
		}
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	return (SUCCESS);
}

static int	setup_next_pipe(t_cmd *cmd, int *prev_pipe)
{
	int	pipe_fds[2];

	if (cmd->next)
	{
		if (pipe(pipe_fds) < 0)
		{
			ft_perror("pipe");
			return (ERROR);
		}
		prev_pipe[0] = pipe_fds[0];
		prev_pipe[1] = pipe_fds[1];
		if (dup2(pipe_fds[1], STDOUT_FILENO) < 0)
		{
			ft_perror("dup2");
			close(pipe_fds[0]);
			close(pipe_fds[1]);
			return (ERROR);
		}
		close(pipe_fds[1]);
	}
	return (SUCCESS);
}

int	setup_pipes(t_cmd *cmd, int *prev_pipe)
{
	if (setup_prev_pipe(prev_pipe) == ERROR)
		return (ERROR);
	if (setup_next_pipe(cmd, prev_pipe) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
