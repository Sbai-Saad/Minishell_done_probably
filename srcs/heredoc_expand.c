/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:00:00 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/16 00:00:00 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	hd_process_dollar(struct s_buf *out, const char **pp,
				t_shell *shell)
{
	size_t		adv;

	if (!try_expand_at(out, *pp, shell, &adv))
		return (0);
	*pp += adv;
	return (1);
}

static int	hd_append_char(struct s_buf *out, const char **pp)
{
	if (!buf_putn(out, *pp, 1))
		return (0);
	(*pp)++;
	return (1);
}

static int	hd_expand_iterate(struct s_buf *out, const char *s,
				t_shell *shell)
{
	const char	*p;

	p = s;
	while (*p)
	{
		if (*p == '$')
		{
			if (!hd_process_dollar(out, &p, shell))
				return (0);
		}
		else
		{
			if (!hd_append_char(out, &p))
				return (0);
		}
	}
	return (1);
}

char	*expand_heredoc_line(const char *s, t_shell *shell)
{
	struct s_buf	out;

	if (!s)
		return (NULL);
	if (!buf_init(&out, ft_strlen(s) + 32))
		return (NULL);
	if (!hd_expand_iterate(&out, s, shell))
	{
		free(out.data);
		return (NULL);
	}
	return (out.data);
}

int	buf_putn(struct s_buf *b, const char *s, size_t n)
{
	if (b->len + n + 1 > b->cap)
	{
		if (!buf_grow(b, n))
			return (0);
	}
	if (n)
		ft_memcpy(b->data + b->len, s, n);
	b->len += n;
	b->data[b->len] = '\0';
	return (1);
}
