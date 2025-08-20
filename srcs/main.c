/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:03:29 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/12 10:03:29 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_received = 0;

void	cleanup_shell(t_shell *shell)
{
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->cmds)
	{
		free_commands(shell->cmds);
		shell->cmds = NULL;
	}
	if (isatty(STDIN_FILENO))
		rl_clear_history();
	free_env(shell->env_list);
}

static int	get_input_line(t_shell *shell, char **out_input)
{
	const char	*prompt;

	(void)shell;
	{
		if (isatty(STDIN_FILENO))
			prompt = PROMPT;
		else
			prompt = "";
		*out_input = readline(prompt);
	}
	if (!*out_input)
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		return (1);
	}
	return (0);
}

static int	handle_signal_and_empty(char **input)
{
	if (g_signal_received == SIGINT || g_signal_received == SIGQUIT)
	{
		g_signal_received = 0;
		if ((*input)[0] == '\0')
		{
			free(*input);
			*input = NULL;
			return (1);
		}
	}
	return (0);
}

void	run_shell(t_shell *shell)
{
	char	*input;
	int		ret;

	while (shell->running)
	{
		ret = get_input_line(shell, &input);
		if (ret == -1)
			continue ;
		if (ret == 1)
			break ;
		if (handle_signal_and_empty(&input))
			continue ;
		process_input_line(shell, input, isatty(STDIN_FILENO));
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		final_status;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	run_shell(&shell);
	final_status = shell.exit_status;
	cleanup_shell(&shell);
	return (final_status);
}
