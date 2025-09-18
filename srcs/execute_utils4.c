/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:02:16 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/21 06:02:16 by sasbai           ###   ########.fr       */
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
		errno = 0;
		pid = wait(&status);
		if (pid < 0)
		{
			if (errno == EINTR)
				continue ;
			break ;
		}
		if (pid == 0)
			continue ;
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

void	set_echoctl(int enable)
{
	struct termios	term;

	if (!isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag = (term.c_lflag & ~ECHOCTL) | ((enable != 0) * ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
