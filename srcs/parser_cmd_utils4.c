/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:00:53 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/21 06:00:53 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int
	choose_redir_name_single(const char *word, char *expanded, t_shell *shell,
	char **out_name)
{
	int		dummy;
	char	**fields;

	fields = split_unquoted_to_array(expanded, &dummy);
	if (!fields)
	{
		ft_error("minishell: ");
		ft_error((char *)word);
		ft_error(": ambiguous redirect\n");
		shell->exit_status = 1;
		return (-1);
	}
	if (fields[0][0] == '\0')
	{
		ft_error("minishell: ");
		ft_error((char *)word);
		ft_error(": ambiguous redirect\n");
		shell->exit_status = 1;
		free(fields[0]);
		free(fields);
		return (-1);
	}
	*out_name = fields[0];
	free(fields);
	return (0);
}

int	has_quotes(const char *s)
{
	while (s && *s)
	{
		if (*s == '\'' || *s == '"')
			return (1);
		s++;
	}
	return (0);
}

int	count_unquoted_fields(const char *s)
{
	int	count;
	int	i;
	int	in_sq;
	int	in_dq;

	count = 0;
	i = 0;
	in_sq = 0;
	in_dq = 0;
	if (!s || !s[0])
		return (0);
	while (s[i])
	{
		skip_ws_outside_quotes(s, &i, in_sq, in_dq);
		if (!s[i])
			break ;
		count++;
		advance_over_field(s, &i, &in_sq, &in_dq);
	}
	return (count);
}

int
	advance_over_field(const char *s, int *i, int *in_sq, int *in_dq)
{
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
	return (1);
}

void	skip_ws_outside_quotes(const char *s, int *i, int in_sq, int in_dq)
{
	while (s[*i] && !in_sq && !in_dq && is_whitespace(s[*i]))
		(*i)++;
}
