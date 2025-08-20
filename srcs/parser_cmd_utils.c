/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/08/16 00:00:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*
	extract_raw_field(const char *s, int *i, int *in_sq, int *in_dq)
{
	int		start;

	start = *i;
	while (s[*i])
	{
		if (!*in_dq && s[*i] == '\'')
		{
			*in_sq = !*in_sq;
			(*i)++;
			continue ;
		}
		if (!*in_sq && s[*i] == '"')
		{
			*in_dq = !*in_dq;
			(*i)++;
			continue ;
		}
		if (!*in_sq && !*in_dq && is_whitespace(s[*i]))
			break ;
		(*i)++;
	}
	return (ft_strndup(s + start, (size_t)(*i - start)));
}

char	*
	extract_next_unquoted_field(const char *s, int *i, int *in_sq, int *in_dq)
{
	char	*raw;
	char	*final;

	skip_ws_outside_quotes(s, i, *in_sq, *in_dq);
	if (!s[*i])
		return (NULL);
	raw = extract_raw_field(s, i, in_sq, in_dq);
	final = remove_quotes(raw);
	free(raw);
	return (final);
}

static void
	fill_unquoted_array(const char *s, int count, char **arr)
{
	int		i;
	int		in_sq;
	int		in_dq;
	int		idx;
	char	*field;

	i = 0;
	in_sq = 0;
	in_dq = 0;
	idx = 0;
	while (s && s[i] && idx < count)
	{
		field = extract_next_unquoted_field(s, &i, &in_sq, &in_dq);
		if (!field)
			break ;
		arr[idx++] = field;
	}
}

char	**
	split_unquoted_to_array(const char *s, int *out_count)
{
	int		count;
	char	**arr;

	count = count_unquoted_fields(s);
	*out_count = count;
	if (count == 0)
		return (NULL);
	arr = (char **)ft_malloc(sizeof(char *) * (count));
	fill_unquoted_array(s, count, arr);
	return (arr);
}
