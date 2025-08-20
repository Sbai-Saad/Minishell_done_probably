/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 09:59:51 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/12 09:59:51 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_words(char const *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (count);
}

static char	*get_next_word(char const **s, char c)
{
	const char	*start;
	int			len;

	while (**s && **s == c)
		(*s)++;
	start = *s;
	len = 0;
	while ((*s)[len] && (*s)[len] != c)
		len++;
	*s += len;
	return (ft_strndup(start, len));
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		word_count;
	int		i;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	split = ft_malloc(sizeof(char *) * (word_count + 1));
	i = 0;
	while (i < word_count)
	{
		split[i] = get_next_word(&s, c);
		i++;
	}
	split[i] = NULL;
	return (split);
}
