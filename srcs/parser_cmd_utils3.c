/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:00:45 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/21 06:00:45 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	choose_redir_name(const char *word, char *expanded,
	t_shell *shell, char **out_name)
{
	int	n;

	n = count_unquoted_fields(expanded);
	if (n == 1)
		return (choose_redir_name_single(word, expanded, shell, out_name));
	ft_error("minishell: ");
	ft_error((char *)word);
	ft_error(": ambiguous redirect\n");
	shell->exit_status = 1;
	return (-1);
}

int	resolve_redir_target(const char *word, t_shell *shell, char **out_name)
{
	char	*expanded;
	int		rc;

	expanded = expand_variables((char *)word, shell);
	rc = choose_redir_name(word, expanded, shell, out_name);
	free(expanded);
	return (rc);
}

int	handle_in_redirection(t_cmd *cmd, t_token *token, t_shell *shell)
{
	char	*name;

	if (token->type != TOKEN_REDIR_IN)
		return (0);
	name = NULL;
	if (resolve_redir_target(token->next->value, shell, &name) < 0)
		return (-1);
	add_inredir(cmd, name);
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = name;
	return (1);
}

int	handle_out_or_append_redirection(t_cmd *cmd, t_token *token,
	t_shell *shell)
{
	char	*name;

	if (token->type != TOKEN_REDIR_OUT && token->type != TOKEN_REDIR_APPEND)
		return (0);
	name = NULL;
	if (resolve_redir_target(token->next->value, shell, &name) < 0)
		return (-1);
	if (token->type == TOKEN_REDIR_OUT)
		add_outredir(cmd, name, 0);
	else
		add_outredir(cmd, name, 1);
	free(name);
	return (1);
}
