/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 00:14:41 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/09/21 00:14:41 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(t_cmd *cmd, t_env *t_env)
{
	int		exit_code;

	exit_code = 0;
	if (cmd->cmd->args[1] != NULL)
	{
		printf("minishell\nexit: too many arguments\n");
		return (1);
	}
	if (cmd->cmd->args[0] != NULL)
	{
		if (ft_isdigit(cmd->cmd->args[0][0]) || cmd->cmd->args[0][0] == '-')
			exit_code = ft_atoi(cmd->cmd->args[0]);
		else
		{
			printf("minishell\nexit: %s: numeric argument required\n",
				cmd->cmd->args[0]);
			exit_code = 2;
		}
	}
	env_free(t_env);
	cmd_free(cmd);
	exit(exit_code);
}
