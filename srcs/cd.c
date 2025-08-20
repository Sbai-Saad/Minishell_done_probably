/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:18:13 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:18:13 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*cd_target_path(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	if (!cmd->args[1])
	{
		path = get_env_value(shell->env_list, "HOME");
		if (!path)
		{
			ft_error("cd: HOME not set\n");
			return (NULL);
		}
	}
	else
		path = cmd->args[1];
	return (path);
}

static void	cd_set_oldpwd(t_shell *shell, char *oldcwd)
{
	char	*oldpwd_env;

	if (oldcwd[0] != '\0')
		oldpwd_env = oldcwd;
	else
		oldpwd_env = get_env_value(shell->env_list, "PWD");
	if (oldpwd_env)
		set_env_value(&shell->env_list, "OLDPWD", (char *)oldpwd_env);
}

static void	cd_set_pwd(t_shell *shell, char *fallback_path)
{
	char	newcwd[1024];

	if (getcwd(newcwd, sizeof(newcwd)))
		set_env_value(&shell->env_list, "PWD", newcwd);
	else if (fallback_path)
		set_env_value(&shell->env_list, "PWD", fallback_path);
}

int	builtin_cd(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	oldcwd[1024];

	path = cd_target_path(shell, cmd);
	if (!path)
		return (1);
	if (!getcwd(oldcwd, sizeof(oldcwd)))
		oldcwd[0] = '\0';
	if (chdir(path) != 0)
	{
		ft_perror("cd");
		return (1);
	}
	cd_set_oldpwd(shell, oldcwd);
	cd_set_pwd(shell, path);
	return (0);
}
