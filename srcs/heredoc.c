/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:03:18 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/12 10:03:18 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	build_heredoc_fd(char *delimiter, int quoted, t_shell *shell)
{
	int				pipefd[2];
	pid_t			pid;

	if (pipe(pipefd) == -1)
	{
		ft_perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid < 0)
	{
		ft_perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_child(pipefd[1], delimiter, quoted, shell);
	}
	close(pipefd[1]);
	return (heredoc_wait_and_result(pid, pipefd[0]));
}

static t_heredoc	*get_last_heredoc(t_heredoc *head)
{
	t_heredoc	*last;

	last = NULL;
	while (head)
	{
		last = head;
		head = head->next;
	}
	return (last);
}

static int	build_all_heredocs(t_cmd *cmd, t_shell *shell, t_heredoc *last)
{
	int				rfd;
	t_heredoc		*h;

	h = cmd->heredocs;
	while (h)
	{
		rfd = build_heredoc_fd(h->eof, h->quoted, shell);
		if (rfd == -2)
		{
			shell->exit_status = 1;
			g_signal_received = 1;
			return (-1);
		}
		if (rfd < 0)
		{
			g_signal_received = 1;
			return (-1);
		}
		if (h == last)
			cmd->heredoc_fd = rfd;
		else
			close(rfd);
		h = h->next;
	}
	return (0);
}

static int	process_all_heredocs(t_cmd *cmd, t_shell *shell)
{
	t_heredoc	*last;

	if (cmd->heredoc_fd != -1)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	last = get_last_heredoc(cmd->heredocs);
	if (!last)
		return (0);
	return (build_all_heredocs(cmd, shell, last));
}

void	handle_heredocs(t_cmd *cmds, t_shell *shell)
{
	t_cmd			*current;

	ignore_signals();
	current = cmds;
	while (current)
	{
		if (g_signal_received)
		{
			setup_signals();
			return ;
		}
		if (current->heredocs)
		{
			if (process_all_heredocs(current, shell) != 0)
			{
				setup_signals();
				return ;
			}
		}
		current = current->next;
	}
	setup_signals();
}
