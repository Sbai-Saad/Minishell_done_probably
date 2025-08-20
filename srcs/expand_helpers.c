/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:19:22 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:19:22 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	append_sanitized(char **pptr, const char *val)
{
	char	*dst;

	dst = *pptr;
	while (*val)
	{
		if (*val == '\'')
			*dst++ = (char)0x1E;
		else if (*val == '"')
			*dst++ = (char)0x1F;
		else
			*dst++ = *val;
		val++;
	}
	*pptr = dst;
}

int	try_handle_quote(const char **current, char **ptr, int *in_sq, int *in_dq)
{
	if (**current == '\'' && !(*in_dq))
	{
		*in_sq = !(*in_sq);
		*(*ptr)++ = *(*current)++;
		return (1);
	}
	if (**current == '"' && !(*in_sq))
	{
		*in_dq = !(*in_dq);
		*(*ptr)++ = *(*current)++;
		return (1);
	}
	return (0);
}

int	try_handle_dollar(const char **current, char **ptr, int in_sq,
		t_shell *shell)
{
	char	*var_val;

	if (**current == '$' && !in_sq)
	{
		var_val = get_var_value(current, shell);
		append_sanitized(ptr, var_val);
		free(var_val);
		return (1);
	}
	return (0);
}
