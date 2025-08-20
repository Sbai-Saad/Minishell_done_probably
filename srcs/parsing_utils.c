/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:04:09 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/12 10:04:09 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	int	check_leading_pipe(t_token *head)
{
	if (head && head->type == TOKEN_PIPE)
	{
		ft_error("syntax error near unexpected token `|'\n");
		return (0);
	}
	return (1);
}

static int	validate_current_token(t_token *cur)
{
	if (is_redir_token(cur))
	{
		if (!cur->next || cur->next->type != TOKEN_WORD)
		{
			ft_error("syntax error near unexpected token `newline'\n");
			return (0);
		}
	}
	if (cur->type == TOKEN_PIPE)
	{
		if (!cur->next || cur->next->type == TOKEN_PIPE)
		{
			ft_error("syntax error near unexpected token `|'\n");
			return (0);
		}
	}
	return (1);
}

int	is_valid_syntax(t_token *tokens)
{
	t_token	*cur;

	if (!check_leading_pipe(tokens))
		return (0);
	cur = tokens;
	while (cur)
	{
		if (!validate_current_token(cur))
			return (0);
		cur = cur->next;
	}
	return (1);
}

void	skip_ws_outside_quotes2(const char *s, int *i, int in_sq, int in_dq)
{
	while (s[*i] && !in_sq && !in_dq && is_whitespace(s[*i]))
		(*i)++;
}

void	advance_over_field2(const char *s, int *i, int *in_sq, int *in_dq)
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
}
