/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:18:02 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:18:02 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_shell *shell, t_cmd *cmd)
{
	char	*builtin;

	builtin = cmd->args[0];
	if (ft_strcmp(builtin, "echo") == 0)
		return (builtin_echo(cmd));
	if (ft_strcmp(builtin, "cd") == 0)
		return (builtin_cd(shell, cmd));
	if (ft_strcmp(builtin, "pwd") == 0)
		return (builtin_pwd(shell));
	if (ft_strcmp(builtin, "export") == 0)
		return (builtin_export(shell, cmd));
	if (ft_strcmp(builtin, "unset") == 0)
		return (builtin_unset(shell, cmd));
	if (ft_strcmp(builtin, "env") == 0)
		return (builtin_env(shell));
	if (ft_strcmp(builtin, "exit") == 0)
		return (builtin_exit(shell, cmd));
	return (1);
}
