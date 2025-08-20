/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:19:28 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:19:28 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_valid_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

char	*handle_dollar_special(const char **s_ptr, t_shell *shell)
{
	const char	*s;

	s = *s_ptr + 1;
	if (*s == '?')
	{
		*s_ptr += 2;
		return (ft_itoa(shell->exit_status));
	}
	if (*s >= '0' && *s <= '9')
	{
		*s_ptr += 2;
		return (ft_strdup(""));
	}
	return (NULL);
}

char	*handle_no_var_name(const char **s_ptr, const char *s)
{
	if (*s == '"' || *s == '\'')
	{
		*s_ptr += 1;
		return (ft_strdup(""));
	}
	*s_ptr += 1;
	return (ft_strdup("$"));
}

char	*get_var_value(const char **s_ptr, t_shell *shell)
{
	const char	*s;
	char		*var_name;
	char		*var_value;
	int			len;
	char		*special;

	special = handle_dollar_special(s_ptr, shell);
	if (special)
		return (special);
	s = *s_ptr + 1;
	len = 0;
	while (is_valid_var_char(s[len]))
		len++;
	if (len == 0)
		return (handle_no_var_name(s_ptr, s));
	var_name = ft_strndup(s, len);
	var_value = get_env_value(shell->env_list, var_name);
	free(var_name);
	*s_ptr += len + 1;
	if (var_value)
		return (ft_strdup(var_value));
	return (ft_strdup(""));
}
