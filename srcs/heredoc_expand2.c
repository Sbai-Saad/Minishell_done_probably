/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:00:10 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/21 06:00:10 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	buf_init(struct s_buf *b, size_t cap_hint)
{
	if (cap_hint < 64)
		cap_hint = 64;
	b->data = (char *)ft_malloc(cap_hint);
	if (!b->data)
		return (0);
	b->len = 0;
	b->cap = cap_hint;
	b->data[0] = '\0';
	return (1);
}

int	buf_grow(struct s_buf *b, size_t min_extra)
{
	size_t			need;
	size_t			new_cap;
	char			*new_data;

	need = b->len + min_extra + 1;
	new_cap = b->cap;
	if (new_cap == 0)
		new_cap = 64;
	while (new_cap < need)
		new_cap *= 2;
	new_data = (char *)ft_malloc(new_cap);
	if (!new_data)
		return (0);
	if (b->len)
		ft_memcpy(new_data, b->data, b->len);
	new_data[b->len] = '\0';
	free(b->data);
	b->data = new_data;
	b->cap = new_cap;
	return (1);
}

int	hd_expand_eof(struct s_buf *out, const char *p, size_t *adv)
{
	if (!buf_putn(out, p, 1))
		return (0);
	*adv = 1;
	return (1);
}

int	try_expand_at(struct s_buf *out, const char *p,
				t_shell *shell, size_t *adv)
{
	const char	*q;

	q = p + 1;
	if (*q == '\0')
		return (hd_expand_eof(out, p, adv));
	if (*q == '?')
		return (hd_expand_qmark(out, shell, adv));
	if (*q == '_' || ft_isalpha((unsigned char)*q))
		return (hd_expand_var(out, q, shell, adv));
	return (hd_expand_literal_dollar(out, p, adv));
}
