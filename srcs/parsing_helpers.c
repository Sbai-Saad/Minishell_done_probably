/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:04:02 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/12 10:04:02 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_args_from_tokens(t_token **tokens, t_shell *shell)
{
	int		total_fields;
	t_token	*current;
	char	*expanded_val;

	total_fields = 0;
	current = *tokens;
	while (current)
	{
		expanded_val = expand_variables(current->value, shell);
		total_fields += count_unquoted_fields2(expanded_val);
		free(expanded_val);
		current = current->next;
	}
	return (total_fields);
}

static int
	fill_args_from_tokens(t_token **tokens, t_shell *shell, char **args)
{
	t_token	*current;
	char	*expanded_val;
	int		idx;

	idx = 0;
	current = *tokens;
	while (current)
	{
		expanded_val = expand_variables(current->value, shell);
		split_unquoted_and_fill(expanded_val, args, &idx);
		free(expanded_val);
		current = current->next;
	}
	return (idx);
}

char	**extract_args(t_token **tokens, t_shell *shell)
{
	int		total_fields;
	char	**args;
	int		idx;

	total_fields = count_args_from_tokens(tokens, shell);
	args = ft_malloc(sizeof(char *) * (total_fields + 1));
	idx = fill_args_from_tokens(tokens, shell, args);
	args[idx] = NULL;
	return (args);
}

void	init_cmd(t_cmd *cmd)
{
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->heredoc_fd = -1;
	cmd->heredocs = NULL;
	cmd->in_redirs = NULL;
	cmd->out_redirs = NULL;
	cmd->next = NULL;
}

void	add_command_to_list(t_cmd **cmds, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!*cmds)
	{
		*cmds = new_cmd;
		return ;
	}
	current = *cmds;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}
