/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:18:27 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:18:27 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*init_env(char **envp)
{
	t_env	*head;
	int		i;
	char	*key;
	char	*value;
	int		append;

	if (!envp || !*envp)
		return (NULL);
	head = NULL;
	i = 0;
	while (envp[i])
	{
		parse_env_var(envp[i], &key, &value, &append);
		set_env_value(&head, key, value);
		free(key);
		if (value)
			free(value);
		i++;
	}
	return (head);
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static int	env_update_if_exists(t_env *env, char *key, char *value)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (value == NULL)
				return (1);
			free(cur->value);
			cur->value = ft_strdup(value);
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}

static void	env_append_new(t_env **env_list, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = *env_list;
	while (current && current->next)
		current = current->next;
	new_node = ft_malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	if (*env_list == NULL)
		*env_list = new_node;
	else
		current->next = new_node;
}

void	set_env_value(t_env **env_list, char *key, char *value)
{
	if (env_update_if_exists(*env_list, key, value))
		return ;
	env_append_new(env_list, key, value);
}
