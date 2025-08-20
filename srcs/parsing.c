/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:04:15 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/12 10:04:15 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*parse_commands(t_token *tokens, t_shell *shell)
{
	t_cmd	*cmds;
	t_cmd	*new_cmd;
	t_token	*current_token;
	t_token	*pipe_token;

	cmds = NULL;
	current_token = tokens;
	while (current_token)
	{
		new_cmd = create_next_command(&current_token, &pipe_token, shell);
		if (!new_cmd)
			return (handle_create_failure_and_null(shell, cmds));
		add_command_to_list(&cmds, new_cmd);
		if (pipe_token)
			current_token = pipe_token->next;
		else
			current_token = NULL;
	}
	return (cmds);
}

static	int	check_unclosed_quotes(const char *input, char *unclosed)
{
	char	q;
	size_t	i;

	q = 0;
	i = 0;
	while (input && input[i])
	{
		if ((input[i] == '\'' || input[i] == '"'))
		{
			if (q == 0)
				q = input[i];
			else if (q == input[i])
				q = 0;
		}
		i++;
	}
	if (q)
	{
		*unclosed = q;
		return (1);
	}
	return (0);
}

int	parse_input(t_shell *shell, char *input)
{
	char	uq;

	if (check_unclosed_quotes(input, &uq))
	{
		ft_error("bash: -c: line 1: unexpected EOF while looking for ");
		ft_error("matching `");
		write(2, &uq, 1);
		ft_error("'\n");
		ft_error("bash: -c: line 2: syntax error: unexpected end of file\n");
		shell->exit_status = 2;
		return (0);
	}
	shell->tokens = tokenize_input(input);
	if (!shell->tokens)
		return (0);
	if (!is_valid_syntax(shell->tokens))
	{
		shell->exit_status = 2;
		return (0);
	}
	shell->cmds = parse_commands(shell->tokens, shell);
	if (!shell->cmds)
		return (0);
	return (1);
}

static void	free_command(t_cmd *cmd)
{
	t_heredoc	*h;
	t_heredoc	*hn;

	ft_free_arr(cmd->args);
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->heredocs)
	{
		h = cmd->heredocs;
		while (h)
		{
			hn = h->next;
			if (h->eof)
				free(h->eof);
			free(h);
			h = hn;
		}
	}
	if (cmd->heredoc_fd != -1)
		close(cmd->heredoc_fd);
	if (cmd->out_redirs)
		free_out_redirs(cmd->out_redirs);
	if (cmd->in_redirs)
		free_in_redirs(cmd->in_redirs);
	free(cmd);
}

void	free_commands(t_cmd *cmds)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmds;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}
