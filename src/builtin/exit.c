/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 00:14:41 by julio.formi       #+#    #+#             */
/*   Updated: 2025/02/02 22:08:44 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_exit_code(char **args, int i)
{
	if (!args)
		return (0);
	if (args[0])
	{
		while (ft_isdigit(args[0][i]))
			i++;
		if (args[0][i] == '\0')
		{
			if (ft_array_len(args) > 1)
			{
				printf("minishell: exit: too many arguments\n");
				return (1);
			}
		}
		else
		{
			printf("minishell: exit: %s: numeric argument required\n", args[0]);
			return (2);
		}
		return (ft_atoi(args[0]));
	}
	return (0);
}

int	builtin_exit(t_cmd *cmd, t_env *env)
{
	int	exit_code;
	int	i;

	if (cmd->cmd->args && cmd->cmd->args[0][0] == '-')
		i = 1;
	else
		i = 0;
	printf("exit\n");
	exit_code = parse_exit_code(cmd->cmd->args, i);
	if (exit_code == 1)
		return (exit_code);
	rl_clear_history();
	free_cmd(cmd);
	env_free(env);
	exit(exit_code % 256);
}
