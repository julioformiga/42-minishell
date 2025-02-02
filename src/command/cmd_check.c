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
	while (env)
	{
		if (ft_strcmp(env->key, "PATH") == 0)
		{
			paths = ft_split(env->value, ':');
			break ;
		}
		env = env->next;
	}
	if (!paths)
	{
		g_signal = 127;
		return (NULL);
	}
	return (paths);
}

static int	check_special_path(char *cmd_name)
{
	if (cmd_name == NULL || !(cmd_name[0] == '/' || cmd_name[0] == '.'))
		return (1);
	if (ft_strcmp(cmd_name, ".") == 0 || ft_strcmp(cmd_name, "..") == 0
		|| ft_strcmp(cmd_name, "./") == 0 || ft_strcmp(cmd_name, "../") == 0
		|| ft_strcmp(cmd_name, "/") == 0)
	{
		g_signal = 126;
		return (1);
	}
	return (0);
}

static char	*check_absolute_path(char *cmd_name)
{
	struct stat	path_stat;

	if (check_special_path(cmd_name))
		return (NULL);
	if (stat(cmd_name, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			g_signal = 126;
			return (NULL);
		}
		if (access(cmd_name, X_OK) == 0)
			return (cmd_name);
		g_signal = 126;
		return (NULL);
	}
	g_signal = 127;
	return (NULL);
}

static char	*find_command_path(char **paths, char *cmd_name)
{
	char	*full_path;
	char	*full_path_cmd;
	char	*result;
	int		i;

	result = check_absolute_path(cmd_name);
	if (paths == NULL || !result)
		return (result);
	i = -1;
	while (paths[++i] && !result)
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path_cmd = ft_strjoin(full_path, cmd_name);
		free(full_path);
		if (access(full_path_cmd, X_OK) == 0)
			result = full_path_cmd;
		else
			free(full_path_cmd);
	}
	return (result);
}

char	*cmd_check(t_cmd *cmd, t_env *env)
{
	char	**paths;
	char	*result;

	paths = get_paths(env);
	result = find_command_path(paths, cmd->cmd->exec);
	if (paths)
		free_array(paths);
	if (!result)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd->exec, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free_cmd(cmd);
		env_free(env);
		g_signal = 127;
	}
	return (result);
}
