/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbenhamd <lbenhamd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 01:19:18 by lbenhamd          #+#    #+#             */
/*   Updated: 2025/08/18 01:19:18 by lbenhamd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	trim_and_get_len(const char *s, const char **out_s,
		size_t *out_len)
{
	const char	*start;
	const char	*end;
	size_t		len;

	start = s;
	end = s + ft_strlen(s);
	trim_spaces(&start, &end);
	len = (size_t)(end - start);
	if (len == 0)
		return (0);
	*out_s = start;
	*out_len = len;
	return (1);
}

static int	parse_optional_sign(const char **ps, size_t *plen,
		long long *out_sign)
{
	const char	*s;
	size_t		len;
	long long	sign;
	size_t		i;

	s = *ps;
	len = *plen;
	sign = 1;
	i = 0;
	if (i < len && (s[i] == '+' || s[i] == '-'))
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	if (i >= len)
		return (0);
	*out_sign = sign;
	*ps = s + i;
	*plen = len - i;
	return (1);
}

static int	apply_sign_to_value(long long sign, unsigned long long val,
		int *out_int)
{
	long long	signed_val;

	if (sign == 1)
	{
		if (val > LLONG_MAX)
			return (0);
		signed_val = (long long)val;
	}
	else
	{
		if (val > (unsigned long long)LLONG_MAX + 1ULL)
			return (0);
		signed_val = -(long long)val;
	}
	*out_int = (int)(signed_val & 0xFF);
	return (1);
}

static int	parse_exit_code(const char *s, int *out)
{
	long long			sign;
	unsigned long long	val;
	const char			*s_trim;
	size_t				len;

	val = 0ULL;
	if (!trim_and_get_len(s, &s_trim, &len))
		return (0);
	if (!parse_optional_sign(&s_trim, &len, &sign))
		return (0);
	if (!accumulate_value(s_trim, len, &val))
		return (0);
	if (!apply_sign_to_value(sign, val, out))
		return (0);
	return (1);
}

int	builtin_exit(t_shell *shell, t_cmd *cmd)
{
	int	status;

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		write(2, "exit\n", 5);
	shell->running = 0;
	if (cmd->args[1])
	{
		if (!is_str_numeric(cmd->args[1])
			|| !parse_exit_code(cmd->args[1], &status))
		{
			write(2, "exit: ", 6);
			write(2, cmd->args[1], ft_strlen(cmd->args[1]));
			write(2, ": numeric argument required\n", 28);
			shell->exit_status = 2;
			return (shell->exit_status);
		}
		if (cmd->args[2])
		{
			ft_error("exit: too many arguments\n");
			shell->running = 1;
			return (1);
		}
		shell->exit_status = status;
	}
	return (shell->exit_status);
}
