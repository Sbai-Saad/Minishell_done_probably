/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:19:13 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:19:13 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_blank(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

void	trim_spaces(const char **s_ptr, const char **end_ptr)
{
	const char	*s;
	const char	*e;

	s = *s_ptr;
	e = *end_ptr;
	while (s < e && is_blank(*s))
	{
		s++;
	}
	while (e > s && is_blank(*(e - 1)))
	{
		e--;
	}
	*s_ptr = s;
	*end_ptr = e;
}

int	is_str_numeric(const char *s)
{
	const char	*e;

	e = s + ft_strlen(s);
	trim_spaces(&s, &e);
	if (s >= e)
		return (0);
	if (*s == '+' || *s == '-')
		s++;
	if (s >= e)
		return (0);
	while (s < e)
	{
		if (*s < '0' || *s > '9')
			return (0);
		s++;
	}
	return (1);
}

int	accumulate_value(const char *s, size_t len,
		unsigned long long *out_val)
{
	unsigned long long	v;
	unsigned int		digit;
	size_t				i;

	v = 0ULL;
	i = 0;
	while (i < len)
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		digit = (unsigned)(s[i] - '0');
		if (v > ULLONG_MAX / 10ULL)
			return (0);
		if (v == ULLONG_MAX / 10ULL
			&& digit > ULLONG_MAX % 10ULL)
			return (0);
		v = v * 10ULL + digit;
		i++;
	}
	*out_val = v;
	return (1);
}
