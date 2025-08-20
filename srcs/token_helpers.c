/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:05:02 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/12 10:05:02 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	process_special_char(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '|')
		add_token(tokens, create_token(ft_strdup("|"), TOKEN_PIPE));
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(tokens, create_token(ft_strdup("<<"), TOKEN_HEREDOC));
		(*i)++;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(tokens, create_token(ft_strdup(">>"), TOKEN_REDIR_APPEND));
		(*i)++;
	}
	else if (input[*i] == '<')
		add_token(tokens, create_token(ft_strdup("<"), TOKEN_REDIR_IN));
	else if (input[*i] == '>')
		add_token(tokens, create_token(ft_strdup(">"), TOKEN_REDIR_OUT));
	(*i)++;
}

char	*extract_word(char *input, int *i)
{
	int		start;
	int		len;
	char	quote;

	start = *i;
	len = 0;
	quote = 0;
	while (input[*i] && (!is_whitespace(input[*i]) || quote)
		&& (!is_special_char(input[*i]) || quote))
	{
		if ((input[*i] == '\'' || input[*i] == '\"'))
		{
			if (quote == 0)
				quote = input[*i];
			else if (quote == input[*i])
				quote = 0;
		}
		(*i)++;
		len++;
	}
	return (ft_strndup(input + start, len));
}
