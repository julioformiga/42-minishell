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
	int		i;

	paths = get_paths(env);
	i = -1;
	while (paths[++i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path_cmd = ft_strjoin(full_path, cmd->cmd->exec);
		free(full_path);
		if (access(full_path_cmd, X_OK) == 0)
			return (full_path_cmd);
		free(full_path_cmd);
	}
	printf("%s: command not found\n", cmd->cmd->exec);
	return (NULL);
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
	new_block->args = malloc(sizeof(char *) * (cmd_count_args(cmd_part) + 1));
	i = 0;
	while (i++, args[i])
		new_block->args[i - 1] = ft_strdup(args[i]);
	new_block->args[i - 1] = NULL;
	free_array(args);
	new_block->sep = NULL;
	new_block->next = NULL;
	return (new_block);
}

void	cmd_init(char *readline, t_cmd *cmd)
{
	char		*separators[] = {"|", "||", ">", ">>", "<", "<<", "&&", "&", NULL};
	t_cmdblock	*head;
	t_cmdblock	*current;
	char		*start;
	char		*end;
	int			i;
	int			found_separator;
	char		*cmd_part;
	t_cmdblock	*new_block;

	cmd->cmd_line = ft_strdup(readline);
	head = NULL;
	current = NULL;
	cmd_part = NULL;
	new_block = NULL;
	start = readline;
	while (*start)
	{
		while (*start && isspace(*start))
			start++;
		if (!*start)
			break ;
		end = start;
		while (*end)
		{
			found_separator = 0;
			i = -1;
			while (i++, separators[i])
			{
				if (strncmp(end, separators[i], strlen(separators[i])) == 0)
				{
					found_separator = 1;
					break ;
				}
			}
			if (found_separator)
				break ;
			end++;
		}
		cmd_part = strndup(start, end - start);
		new_block = create_cmdblock(cmd_part);
		free(cmd_part);
		if (!head)
			head = new_block;
		else
			current->next = new_block;
		current = new_block;
		if (*end)
		{
			i = -1;
			while (i++, separators[i])
			{
				if (strncmp(end, separators[i], strlen(separators[i])) == 0)
				{
					current->sep = ft_strdup(separators[i]);
					end += strlen(separators[i]);
					break ;
				}
			}
		}
		start = end;
	}
	cmd->cmd = head;
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
		if (block->sep)
			printf("\t\tSeparator: %s\n", block->sep);
		block = block->next;
	}
}
