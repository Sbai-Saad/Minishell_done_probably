/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:03:54 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/12 10:03:54 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_file_redirection(t_cmd *cmd, t_token *token, t_shell *shell)
{
	int	r;

	r = handle_in_redirection(cmd, token, shell);
	if (r != 0)
		return (r);
	return (handle_out_or_append_redirection(cmd, token, shell));
}

int	handle_heredoc_redirection(t_cmd *cmd, t_token *token)
{
	t_heredoc	*node;
	t_heredoc	*tail;

	if (token->type != TOKEN_HEREDOC)
		return (0);
	{
		node = (t_heredoc *)ft_malloc(sizeof(t_heredoc));
		node->quoted = has_quotes(token->next->value);
		node->eof = remove_quotes(token->next->value);
		node->next = NULL;
		if (!cmd->heredocs)
			cmd->heredocs = node;
		else
		{
			tail = cmd->heredocs;
			while (tail->next)
				tail = tail->next;
			tail->next = node;
		}
	}
	return (1);
}

int	process_redirections(t_cmd *cmd, t_token **current, t_shell *shell)
{
	t_token	*token;
	int		r;

	token = *current;
	r = handle_file_redirection(cmd, token, shell);
	if (r == 0)
		r = handle_heredoc_redirection(cmd, token);
	if (r <= 0)
		return (r);
	*current = (*current)->next->next;
	return (1);
}

int	collect_cmd_body(t_cmd *cmd, struct s_token_range range,
	t_shell *shell, t_token **out_temp_head)
{
	t_token	*current;
	t_token	*temp_head;

	temp_head = NULL;
	current = range.start;
	while (current && current != range.end)
	{
		if (process_or_collect_token(cmd, &current, shell, &temp_head) < 0)
		{
			free_tokens(temp_head);
			*out_temp_head = NULL;
			return (-1);
		}
	}
	*out_temp_head = temp_head;
	return (0);
}

int	process_or_collect_token(t_cmd *cmd, t_token **current,
	t_shell *shell, t_token **temp_head)
{
	int	pr;

	if (is_redir_token(*current))
	{
		pr = process_redirections(cmd, current, shell);
		if (pr < 0)
			return (-1);
		return (0);
	}
	add_token(temp_head, create_token(ft_strdup((*current)->value),
			(*current)->type));
	*current = (*current)->next;
	return (0);
}
