/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 08:34:04 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/09/18 08:34:04 by julio.formiga    ###   ########.fr       */
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
		printf("%s: command not found\n", cmd->cmd->exec);
	return (result);
}

int	cmd_setup(t_cmd *cmd, t_env *env, char ***args, char **full_path)
{
	*full_path = cmd_check(cmd, env);
	if (!*full_path)
		return (1);
	*args = cmd->cmd->args;
	return (0);
}

t_cmdblock	*create_cmdblock(char *cmd_part)
{
	t_cmdblock	*new_block;
	char		**args;
	int			i;

	new_block = malloc(sizeof(t_cmdblock));
	args = ft_split(cmd_part, ' ');
	new_block->exec = ft_strdup(args[0]);
	new_block->args = malloc(sizeof(char *) * (cmd_count_args(cmd_part)));
	i = 0;
	while (i++, args[i])
		new_block->args[i - 1] = ft_strdup(args[i]);
	new_block->args[i - 1] = NULL;
	free_array(args);
	new_block->separator = NULL;
	new_block->next = NULL;
	return (new_block);
}

void	cmd_init(char *readline, t_cmd *cmd)
{
	t_cmdblock	*block;
	char		**cmd_parts;
	int			i;

	cmd->cmd_line = ft_strdup(readline);
	cmd_parts = ft_split(readline, '|');
	i = -1;
	while (cmd_parts[++i])
	{
		if (i == 0)
		{
			cmd->cmd = create_cmdblock(cmd_parts[i]);
			block = cmd->cmd;
		}
		else
		{
			block->next = create_cmdblock(cmd_parts[i]);
			block = block->next;
		}
	}
	free_array(cmd_parts);
}

void	cmd_print(t_cmd *command)
{
	t_cmdblock	*block;
	int			i;

	block = command->cmd;
	printf("Full command: %s\n", command->cmd_line);
	while (block)
	{
		printf("\tCommand: %s\n", block->exec);
		i = -1;
		while (i++, block->args[i] != NULL)
			printf("\t\tArg[%d]: %s\n", i, block->args[i]);
		if (block->separator)
			printf("\t\tSeparator: %s\n", block->separator);
		block = block->next;
	}
}
