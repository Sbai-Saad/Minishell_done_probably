/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:09:42 by marvin            #+#    #+#             */
/*   Updated: 2025/08/16 16:09:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	redirect_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_perror(filename);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		ft_perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	open_output_fd(const char *filename, int append)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
		ft_perror(filename);
	return (fd);
}

int	prepare_in_redirs_until_last(t_inredir *list, t_inredir **last_out)
{
	int			fd;
	t_inredir	*last;

	last = list;
	while (last->next)
		last = last->next;
	while (list && list != last)
	{
		fd = open(list->filename, O_RDONLY);
		if (fd == -1)
		{
			ft_perror(list->filename);
			return (1);
		}
		close(fd);
		list = list->next;
	}
	*last_out = last;
	return (0);
}

int	setup_stdin_from_in_redirs(t_inredir *list)
{
	int			fd;
	t_inredir	*last_in;

	if (prepare_in_redirs_until_last(list, &last_in) != 0)
		return (1);
	fd = open(last_in->filename, O_RDONLY);
	if (fd == -1)
	{
		ft_perror(last_in->filename);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		ft_perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}
