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

static int	parse_exit_code(char **args)
{
	if (!args)
		return (0);
	if (ft_array_len(args) > 1)
	{
		printf("minishell\nexit: too many arguments\n");
		return (1);
	}
	if (args[0])
	{
		if (ft_isdigit(args[0][0]) || args[0][0] == '-')
			return (ft_atoi(args[0]));
		printf("minishell: exit: %s: numeric argument required\n", args[0]);
		return (2);
	}
	return (0);
}

int	builtin_exit(t_cmd *cmd, t_env *env)
{
	int	exit_code;

	exit_code = parse_exit_code(cmd->cmd->args);
	if (exit_code == 1)
		return (exit_code);
	rl_clear_history();
	free_cmd(cmd);
	env_free(env);
	exit(exit_code);
}
