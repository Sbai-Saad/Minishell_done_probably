/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:23:10 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/21 03:23:10 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	heredoc_setup_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
}

int	heredoc_write_line(int wfd, char *line, int quoted, t_shell *shell)
{
	char			*exp;

	if (!quoted)
	{
		exp = expand_heredoc_line(line, shell);
		if (!exp)
			return (0);
		write(wfd, exp, ft_strlen(exp));
		free(exp);
	}
	else
		write(wfd, line, ft_strlen(line));
	write(wfd, "\n", 1);
	return (1);
}
