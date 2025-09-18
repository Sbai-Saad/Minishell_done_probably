/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:00:24 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/21 06:00:24 by sasbai           ###   ########.fr       */
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
	set_echoctl(1);
}
