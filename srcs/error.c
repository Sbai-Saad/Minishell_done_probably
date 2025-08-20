/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:18:35 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:18:35 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
}

void	ft_perror(const char *prefix)
{
	perror(prefix);
}

void	exit_command_not_found(t_cmd *cmd)
{
	ft_error(cmd->args[0]);
	ft_error(": command not found\n");
	exit(127);
}
