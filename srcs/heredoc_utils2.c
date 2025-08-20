/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 23:29:21 by marvin            #+#    #+#             */
/*   Updated: 2025/08/15 23:29:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	hd_expand_qmark(struct s_buf *out, t_shell *shell, size_t *adv)
{
	char		*tmp;

	tmp = ft_itoa(shell->exit_status);
	if (!tmp)
		return (0);
	if (!buf_putn(out, tmp, ft_strlen(tmp)))
	{
		free(tmp);
		return (0);
	}
	free(tmp);
	*adv = 2;
	return (1);
}

int	hd_expand_var(
		struct s_buf *out, const char *q, t_shell *shell, size_t *adv)
{
	size_t		nlen;
	char		*tmp;
	char		*val;

	nlen = 1;
	while (q[nlen] && (ft_isalnum((unsigned char)q[nlen]) || q[nlen] == '_'))
		nlen++;
	tmp = ft_strndup(q, nlen);
	if (!tmp)
		return (0);
	val = get_env_value(shell->env_list, tmp);
	free(tmp);
	if (val)
	{
		if (!buf_putn(out, val, ft_strlen(val)))
			return (0);
	}
	*adv = 1 + nlen;
	return (1);
}

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
			_exit(1);
		}
		free(line);
	}
	close(wfd);
	_exit(0);
}

int	heredoc_wait_and_result(pid_t pid, int rfd)
{
	int				status;

	if (waitpid(pid, &status, 0) < 0)
	{
		ft_perror("waitpid");
		close(rfd);
		return (-1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
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
