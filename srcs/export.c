/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:19:47 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:19:47 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha((unsigned char)str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if (!ft_isalnum((unsigned char)str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	export_invalid(char *arg)
{
	ft_error("minishell: export: `");
	ft_error(arg);
	ft_error("': not a valid identifier\n");
	return (1);
}

static void	set_env_append(t_shell *shell, char *key, char *value)
{
	char	*old;
	char	*newval;

	old = get_env_value(shell->env_list, key);
	if (old)
	{
		if (value)
			newval = ft_strjoin(old, value);
		else
			newval = ft_strdup("");
	}
	else
	{
		if (value)
			newval = ft_strdup(value);
		else
			newval = ft_strdup("");
	}
	set_env_value(&shell->env_list, key, newval);
	if (newval)
		free(newval);
}

static int	export_one(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;
	int		append;

	if (is_valid_identifier(arg))
	{
		parse_env_var(arg, &key, &value, &append);
		if (value == NULL && !append)
			set_env_value(&shell->env_list, key, NULL);
		else if (append)
		{
			set_env_append(shell, key, value);
		}
		else
		{
			set_env_value(&shell->env_list, key, value);
		}
		free(key);
		if (value)
			free(value);
		return (0);
	}
	return (export_invalid(arg));
}

int	builtin_export(t_shell *shell, t_cmd *cmd)
{
	int		i;
	int		ret;

	ret = 0;
	if (!cmd->args[1])
	{
		print_sorted_env(shell->env_list);
		return (0);
	}
	i = 1;
	while (cmd->args[i])
	{
		if (export_one(shell, cmd->args[i]))
			ret = 1;
		i++;
	}
	return (ret);
}
