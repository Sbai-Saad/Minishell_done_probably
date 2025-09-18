/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:02:06 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/21 06:02:06 by sasbai           ###   ########.fr       */
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
