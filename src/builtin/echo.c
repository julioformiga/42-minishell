/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:50:06 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/11/25 14:50:06 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(t_cmd *cmd, t_env *env)
{
	int	i;
	int	no_newline;

	(void)env;
	i = 0;
	no_newline = 0;
	if (cmd->cmd->args)
	{
		if (cmd->cmd->args[i] && ft_strncmp(cmd->cmd->args[i], "-n", 2) == 0)
		{
			no_newline = 1;
			i++;
		}
		while (cmd->cmd->args[i])
		{
			ft_putstr_fd(cmd->cmd->args[i], STDOUT_FILENO);
			if (cmd->cmd->args[++i])
				ft_putchar_fd(' ', STDOUT_FILENO);
		}
	}
	if (!no_newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
