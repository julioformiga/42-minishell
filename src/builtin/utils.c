/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 19:25:53 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/01/03 19:25:53 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_builtin	g_builtins[] = {
{"echo", builtin_echo},
{"cd", builtin_cd},
{"exit", builtin_exit},
{"e", builtin_exit},
{"env", builtin_env},
{"export", builtin_export},
{"unset", builtin_unset},
{NULL, NULL}
};

t_builtin_fn	get_builtin(char *cmd_name)
{
	int	i;

	i = -1;
	while (g_builtins[++i].name)
	{
		if (ft_strncmp(cmd_name, g_builtins[i].name,
				ft_strlen(g_builtins[i].name) + 1) == 0)
			return (g_builtins[i].fn);
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
	return (result);
}

