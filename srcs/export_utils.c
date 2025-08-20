/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:19:42 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:19:42 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sort_env_array(t_env **env_array, int count)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env_array[j]->key, env_array[j + 1]->key) > 0)
			{
				tmp = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static t_env	**fill_env_array(t_env *env, int count)
{
	t_env	**env_array;
	t_env	*current;
	int		i;

	env_array = ft_malloc(sizeof(t_env *) * count);
	current = env;
	i = 0;
	while (current)
	{
		env_array[i++] = current;
		current = current->next;
	}
	return (env_array);
}

static void	print_env_array(t_env **env_array, int count)
{
	int		i;

	i = -1;
	while (++i < count)
	{
		if (env_array[i]->value)
			printf("declare -x %s=\"%s\"\n",
				env_array[i]->key, env_array[i]->value);
		else
			printf("declare -x %s\n", env_array[i]->key);
	}
}

void	print_sorted_env(t_env *env)
{
	t_env	**env_array;
	t_env	*current;
	int		count;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	if (count == 0)
		return ;
	env_array = fill_env_array(env, count);
	sort_env_array(env_array, count);
	print_env_array(env_array, count);
	free(env_array);
}
