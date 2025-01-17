/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:57:19 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/01/14 14:57:19 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_paths(t_env *env)
{
	char	**paths;

	paths = NULL;
	while (env->key)
	{
		if (strcmp(env->key, "PATH") == 0)
		{
			paths = ft_split(env->value, ':');
			break ;
		}
		env = env->next;
	}
	return (paths);
}

char	*cmd_check(t_cmd *cmd, t_env *env)
{
	char	*full_path;
	char	*full_path_cmd;
	char	**paths;
	char	*result;
	int		i;

	paths = get_paths(env);
	if (!paths)
		return (NULL);
	result = NULL;
	i = -1;
	while (paths[++i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path_cmd = ft_strjoin(full_path, cmd->cmd->exec);
		free(full_path);
		if (access(full_path_cmd, X_OK) == 0)
		{
			result = full_path_cmd;
			break ;
		}
		free(full_path_cmd);
	}
	free_array(paths);
	if (!result)
	{
		printf("%s: command not found\n", cmd->cmd->exec);
		g_signal = 127;
	}
	return (result);
}

