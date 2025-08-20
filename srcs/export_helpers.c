/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:19:37 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:19:37 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	parse_env_var(char *arg, char **key, char **value, int *append)
{
	char	*equals_pos;

	*append = 0;
	equals_pos = ft_strchr(arg, '=');
	if (equals_pos)
	{
		if (equals_pos > arg && *(equals_pos - 1) == '+')
		{
			*append = 1;
			*key = ft_strndup(arg, (equals_pos - 1) - arg);
		}
		else
		{
			*key = ft_strndup(arg, equals_pos - arg);
		}
		*value = ft_strdup(equals_pos + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}
