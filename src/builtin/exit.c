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

int	builtin_exit(t_cmd *cmd, t_env *env)
{
	int		exit_code;

	exit_code = 0;
	if (cmd->cmd->args != NULL)
	{
		if (ft_array_len(cmd->cmd->args) > 1)
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
				printf("minishell: exit: %s: numeric argument required\n",
					cmd->cmd->args[0]);
				exit_code = 2;
				return (exit_code);
			}
		}
	}
	rl_clear_history();
	free_cmd(cmd);
	env_free(env);
	exit(exit_code);
}
