/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:04:46 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/12 10:04:46 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	copy_unescaped_char(const char **s, char **d)
{
	if (**s == (char)0x1E)
		*(*d)++ = '\'';
	else if (**s == (char)0x1F)
		*(*d)++ = '"';
	else
		*(*d)++ = **s;
	(*s)++;
}

static void	copy_inside_quotes(const char **s, char **d)
{
	char	quote;

	quote = **s;
	(*s)++;
	while (**s && **s != quote)
		copy_unescaped_char(s, d);
	if (**s == quote)
		(*s)++;
}

char	*remove_quotes(char *str)
{
	char		*new_str;
	char		*d;
	const char	*s;

	if (!str)
		return (NULL);
	new_str = ft_malloc(ft_strlen(str) + 1);
	d = new_str;
	s = str;
	while (*s)
	{
		if (*s == '\'' || *s == '"')
			copy_inside_quotes(&s, &d);
		else
			copy_unescaped_char(&s, &d);
	}
	*d = '\0';
	return (new_str);
}
