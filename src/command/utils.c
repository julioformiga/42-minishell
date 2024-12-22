/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:11:10 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/11/18 15:11:10 by julio.formiga    ###   ########.fr       */
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

char	**env_to_array(t_env *env)
{
	int		count;
	char	**env_array;
	t_env	*current;
	char	*tmp;
	int		i;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = -1;
	while (i++, current)
	{
		tmp = ft_strjoin(current->key, "=");
		env_array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

int	cmd_count_args(char *cmd)
{
	int		count;
	char	*token;

	count = 0;
	token = strtok(cmd, " ");
	while (token != NULL)
	{
		count++;
		token = strtok(NULL, " ");
	}
	return (count);
}

char	**cmd_get_args(char *cmd)
{
	char	**args;
	char	*token;
	int		i;
	char	*cmd_copy;

	cmd_copy = ft_strdup(cmd);
	args = malloc(sizeof(char *) * (cmd_count_args(cmd_copy) + 1));
	if (!args)
	{
		free(cmd_copy);
		return (NULL);
	}
	token = strtok(cmd_copy, " ");
	i = 0;
	while (token != NULL)
	{
		args[i] = ft_strdup(token);
		token = strtok(NULL, " ");
		i++;
	}
	args[i] = NULL;
	free(cmd_copy);
	return (args);
}
