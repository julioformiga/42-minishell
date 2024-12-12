/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 01:38:25 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/09/21 01:38:25 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_home(t_env *env)
{
	char	*dir_home;

	dir_home = env_get(env, "HOME");
	if (chdir(dir_home) == -1)
	{
		printf("dir_home: %s\n", dir_home);
		ft_putstr_fd("cd: HOME not set\n", 2);
		free(dir_home);
		return (1);
	}
	return (0);
}

static int	cd_previous(t_env *env)
{
	char	*dir_old;

	dir_old = env_get(env, "OLDPWD");
	if (chdir(dir_old) == -1)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return (1);
	}
	return (0);
}

static int	cd_directory(char *path)
{
	if (path[ft_strlen(path) - 1] == '/' && ft_strlen(path) > 1)
		path[ft_strlen(path) - 1] = '\0';
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: no such file or directory\n", 2);
		return (1);
	}
	return (0);
}

int	builtin_cd(t_cmd *cmd, t_env *env)
{
	char	*dir;
	int		result;

	result = 0;
	if (!cmd->cmd->args[0])
		result = cd_home(env);
	else if (ft_strncmp(cmd->cmd->args[0], "-", 2) == 0)
		result = cd_previous(env);
	else
		result = cd_directory(cmd->cmd->args[0]);
	if (result == 0)
	{
		dir = env_get(env, "PWD");
		env_set(env, "OLDPWD", dir);
		env_set(env, "PWD", getcwd(NULL, 0));
	}
	return (result);
}
