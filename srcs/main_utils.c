/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:23:06 by marvin            #+#    #+#             */
/*   Updated: 2025/08/17 12:23:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_input_line(t_shell *shell, char *input, int add_history_flag)
{
	if (!input || !input[0])
		return ;
	if (add_history_flag)
		add_history(input);
	if (parse_input(shell, input))
		execute_commands(shell);
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	free_commands(shell->cmds);
	shell->cmds = NULL;
}

void	init_shell(t_shell *shell, char **envp)
{
	shell->env_list = init_env(envp);
	shell->tokens = NULL;
	shell->cmds = NULL;
	shell->exit_status = 0;
	shell->running = 1;
	shell->stdin_copy = -1;
	shell->stdout_copy = -1;
	setup_signals();
}
