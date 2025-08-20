/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:19:33 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:19:33 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_variables(char *str, t_shell *shell)
{
	char		*final_str;
	char		*ptr;
	const char	*current;
	int			in_sq;
	int			in_dq;

	in_sq = 0;
	in_dq = 0;
	final_str = ft_malloc(4096);
	ptr = final_str;
	current = str;
	while (*current)
	{
		if (try_handle_quote(&current, &ptr, &in_sq, &in_dq))
			continue ;
		if (try_handle_dollar(&current, &ptr, in_sq, shell))
			continue ;
		*ptr++ = *current++;
	}
	*ptr = '\0';
	return (final_str);
}
