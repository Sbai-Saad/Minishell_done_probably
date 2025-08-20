/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:04:50 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/12 10:04:50 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	setup_stdin(t_cmd *cmd)
{
	if (cmd->heredoc_fd != -1)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
		{
			ft_perror("dup2");
			close(cmd->heredoc_fd);
			cmd->heredoc_fd = -1;
			return (1);
		}
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
		return (0);
	}
	if (cmd->in_redirs)
		return (setup_stdin_from_in_redirs(cmd->in_redirs));
	if (cmd->infile)
		return (redirect_input(cmd->infile));
	return (0);
}

static int
	prepare_out_redirs_until_last(t_outredir *list, t_outredir **last_out)
{
	int			fd;
	t_outredir	*last;
	t_outredir	*cur;

	last = list;
	while (last->next)
		last = last->next;
	cur = list;
	while (cur && cur != last)
	{
		fd = open_output_fd(cur->filename, cur->append);
		if (fd == -1)
			return (1);
		close(fd);
		cur = cur->next;
	}
	*last_out = last;
	return (0);
}

static int	apply_out_redirections(t_outredir *list)
{
	int			fd;
	t_outredir	*last;

	if (!list)
		return (0);
	if (prepare_out_redirs_until_last(list, &last) != 0)
		return (1);
	fd = open_output_fd(last->filename, last->append);
	if (fd == -1)
		return (1);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		ft_perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	handle_redirections(t_cmd *cmd)
{
	if (setup_stdin(cmd) != 0)
		return (1);
	return (apply_out_redirections(cmd->out_redirs));
}

void	restore_redirections(int stdin_copy, int stdout_copy)
{
	if (stdin_copy != -1)
	{
		if (dup2(stdin_copy, STDIN_FILENO) == -1)
			ft_perror("dup2");
		close(stdin_copy);
	}
	if (stdout_copy != -1)
	{
		if (dup2(stdout_copy, STDOUT_FILENO) == -1)
			ft_perror("dup2");
		close(stdout_copy);
	}
}
