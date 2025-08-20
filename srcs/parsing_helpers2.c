/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 14:35:16 by marvin            #+#    #+#             */
/*   Updated: 2025/08/16 14:35:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_unquoted_fields2(const char *s)
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
		skip_ws_outside_quotes2(s, &i, in_sq, in_dq);
		if (!s[i])
			break ;
		count++;
		advance_over_field2(s, &i, &in_sq, &in_dq);
	}
	return (count);
}

char	*extract_raw_field2(const char *s, int *i, int *in_sq, int *in_dq)
{
	int	start;

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

void	split_unquoted_and_fill(const char *s, char **args, int *idx_ptr)
{
	int		i;
	int		in_sq;
	int		in_dq;
	char	*field_raw;
	char	*field_final;

	i = 0;
	in_sq = 0;
	in_dq = 0;
	while (s && s[i])
	{
		skip_ws_outside_quotes2(s, &i, in_sq, in_dq);
		if (!s[i])
			break ;
		field_raw = extract_raw_field2(s, &i, &in_sq, &in_dq);
		field_final = remove_quotes(field_raw);
		free(field_raw);
		args[*idx_ptr] = field_final;
		(*idx_ptr)++;
	}
}

void	find_pipe_token(t_token **pipe_token, t_token *token_ptr)
{
	*pipe_token = token_ptr;
	while (*pipe_token && (*pipe_token)->type != TOKEN_PIPE)
		*pipe_token = (*pipe_token)->next;
}

int	is_redir_token(t_token *cur)
{
	return (cur->type == TOKEN_REDIR_IN || cur->type == TOKEN_REDIR_OUT
		|| cur->type == TOKEN_REDIR_APPEND || cur->type == TOKEN_HEREDOC);
}
