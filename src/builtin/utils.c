/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 19:25:53 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/12 18:28:38 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_builtin	*get_builtins(void)
{
	static t_builtin	builtins[] = {
	{"echo", builtin_echo},
	{"cd", builtin_cd},
	{"exit", builtin_exit},
	{"e", builtin_exit},
	{"env", builtin_env},
	{"export", builtin_export},
	{"unset", builtin_unset},
	{"pwd", builtin_pwd},
	{NULL, NULL}
	};

	return (builtins);
}

t_builtin_fn	get_builtin(char *cmd_name)
{
	t_builtin	*builtins;
	int			i;

	builtins = get_builtins();
	i = -1;
	while (builtins[++i].name)
	{
		if (ft_strcmp(cmd_name, builtins[i].name) == 0)
			return (builtins[i].fn);
	}
	return (NULL);
}

int	execute_builtin(t_cmd *cmd, t_env *env, int input_fd, int output_fd)
{
	int	stdout_backup;
	int	stdin_backup;
	int	result;

	stdout_backup = dup(STDOUT_FILENO);
	stdin_backup = dup(STDIN_FILENO);
	if (input_fd != STDIN_FILENO)
		dup2(input_fd, STDIN_FILENO);
	if (output_fd != STDOUT_FILENO)
		dup2(output_fd, STDOUT_FILENO);
	result = get_builtin(cmd->cmd->exec)(cmd, env);
	dup2(stdout_backup, STDOUT_FILENO);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdout_backup);
	close(stdin_backup);
	return (g_signal = result, result);
}
