/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:50:06 by julio.formi       #+#    #+#             */
/*   Updated: 2025/02/02 22:19:21 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_arg_param(char *arg, int *i)
{
	int	count;

	count = 1;
	if (arg[0] == '-')
	{
		while (arg[count] == 'n')
			count++;
		if (arg[count] == '\0')
		{
			*i = *i + 1;
			return (1);
		}
	}
	return (0);
}

int	builtin_echo(t_cmd *cmd, t_env *env)
{
	int	i;
	int	no_newline;

	(void)env;
	i = 0;
	no_newline = 0;
	if (cmd->cmd->args)
	{
		if (cmd->cmd->args[i] && check_arg_param(cmd->cmd->args[i], &i))
			no_newline = 1;
		while (cmd->cmd->args[i])
		{
			if (no_newline && check_arg_param(cmd->cmd->args[i], &i))
				continue ;
			ft_putstr_fd(cmd->cmd->args[i], STDOUT_FILENO);
			if (cmd->cmd->args[++i])
				ft_putchar_fd(' ', STDOUT_FILENO);
		}
	}
	if (!no_newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
