/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:01:32 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/21 06:01:32 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_out_redirs(t_outredir *list)
{
	t_outredir	*n;

	while (list)
	{
		n = list->next;
		if (list->filename)
			free(list->filename);
		free(list);
		list = n;
	}
}

void	add_inredir(t_cmd *cmd, const char *filename)
{
	t_inredir	*node;
	t_inredir	*cur;

	node = ft_malloc(sizeof(t_inredir));
	node->filename = ft_strdup(filename);
	node->next = NULL;
	if (!cmd->in_redirs)
	{
		cmd->in_redirs = node;
		return ;
	}
	cur = cmd->in_redirs;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

void	free_in_redirs(t_inredir *list)
{
	t_inredir	*n;

	while (list)
	{
		n = list->next;
		if (list->filename)
			free(list->filename);
		free(list);
		list = n;
	}
}

void	add_outredir(t_cmd *cmd, const char *filename, int append)
{
	t_outredir	*node;
	t_outredir	*cur;

	node = ft_malloc(sizeof(t_outredir));
	node->filename = ft_strdup(filename);
	node->append = append;
	node->next = NULL;
	if (!cmd->out_redirs)
	{
		cmd->out_redirs = node;
		return ;
	}
	cur = cmd->out_redirs;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

t_cmd	*create_command(t_token **tokens, t_token *end, t_shell *shell)
{
	t_cmd					*cmd;
	t_token					*temp_head;
	struct s_token_range	range;

	cmd = ft_malloc(sizeof(t_cmd));
	init_cmd(cmd);
	temp_head = NULL;
	range.start = *tokens;
	range.end = end;
	if (collect_cmd_body(cmd, range, shell, &temp_head) < 0)
	{
		free_commands(cmd);
		return (NULL);
	}
	cmd->args = extract_args(&temp_head, shell);
	free_tokens(temp_head);
	return (cmd);
}
