/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:33:06 by marvin            #+#    #+#             */
/*   Updated: 2025/08/16 16:33:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*handle_create_failure_and_null(t_shell *shell, t_cmd *cmds)
{
	if (shell->exit_status == 0)
		shell->exit_status = 1;
	free_commands(cmds);
	return (NULL);
}

t_cmd	*create_next_command(t_token **current_token,
		t_token **pipe_token, t_shell *shell)
{
	find_pipe_token(pipe_token, *current_token);
	return (create_command(current_token, *pipe_token, shell));
}
