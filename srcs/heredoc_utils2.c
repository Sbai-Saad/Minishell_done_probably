/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:00:17 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/21 06:00:17 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	hd_expand_literal_dollar(struct s_buf *out, const char *p,
		size_t *adv)
{
	if (!buf_putn(out, p, 1))
		return (0);
	*adv = 1;
	return (1);
}

void	heredoc_child(
	int wfd, char *delimiter, int quoted, t_shell *shell)
{
	char			*line;

	heredoc_setup_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (!heredoc_write_line(wfd, line, quoted, shell))
		{
			free(line);
			exit(1);
		}
		free(line);
	}
	close(wfd);
	exit(0);
}

static int	wait_heredoc_child(pid_t pid, int *out_status)
{
	int			status;
	pid_t		w;

	while (1)
	{
		w = waitpid(pid, &status, 0);
		if (w < 0)
		{
			if (errno == EINTR)
				continue ;
			ft_perror("waitpid");
			return (-1);
		}
		break ;
	}
	*out_status = status;
	return (0);
}

static int	finalize_heredoc_result(int status, int rfd)
{
	struct termios	t;
	int				echoctl_on;

	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		echoctl_on = 0;
		if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &t) == 0)
			echoctl_on = (t.c_lflag & ECHOCTL) != 0;
		if (echoctl_on)
			write(STDOUT_FILENO, "\n", 1);
		else
			write(STDOUT_FILENO, "^C\n", 3);
		close(rfd);
		return (-2);
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	{
		close(rfd);
		return (-1);
	}
	return (rfd);
}

int	heredoc_wait_and_result(pid_t pid, int rfd)
{
	int	status;

	if (wait_heredoc_child(pid, &status) != 0)
	{
		close(rfd);
		return (-1);
	}
	return (finalize_heredoc_result(status, rfd));
}
