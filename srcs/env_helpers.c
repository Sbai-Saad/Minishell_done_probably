/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:18:22 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:18:22 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	env_count_nodes(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static char	*env_join_kv(char *key, char *value)
{
	char	*tmp;
	char	*res;

	if (!value)
		return (NULL);
	tmp = ft_strjoin(key, "=");
	res = ft_strjoin(tmp, value);
	free(tmp);
	return (res);
}

void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

char	**env_to_array(t_env *env)
{
	t_env	*current;
	char	**arr;
	int		count;
	int		i;

	count = env_count_nodes(env);
	arr = ft_malloc(sizeof(char *) * (count + 1));
	current = env;
	i = 0;
	while (current)
	{
		arr[i] = env_join_kv(current->key, current->value);
		if (arr[i])
			i++;
		current = current->next;
	}
	arr[i] = NULL;
	return (arr);
}
