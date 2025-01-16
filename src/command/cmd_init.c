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

int	cmd_setup(t_cmd *cmd, t_env *env, char ***args, char **full_path)
{
	int	i;

	*full_path = cmd_check(cmd, env);
	if (!*full_path)
		return (1);
	if (!cmd->cmd || !cmd->cmd->exec)
		return (1);
	if (cmd->cmd->args == NULL)
	{
		*args = malloc(sizeof(char *) * 2);
		if (!*args)
			return (1);
		(*args)[0] = ft_strdup(*full_path);
		if (!(*args)[0])
		{
			free(*args);
			return (1);
		}
		(*args)[1] = NULL;
		return (0);
	}
	*args = malloc(sizeof(char *) * (ft_array_len(cmd->cmd->args) + 2));
	if (!*args)
		return (1);
	(*args)[0] = ft_strdup(*full_path);
	i = -1;
	while (cmd->cmd->args[++i])
	{
		(*args)[i + 1] = ft_strdup(cmd->cmd->args[i]);
		if (!(*args)[i + 1])
		{
			while (i >= 0)
				free((*args)[i--]);
			free(*args);
			return (1);
		}
	}
	(*args)[i + 1] = NULL;
	return (0);
}

t_cmdblock	*create_cmdblock(char *cmd_part)
{
	t_cmdblock	*new_block;
	char		**args;
	int			i;
	int			arg_count;


	if (!cmd_part)
		return (NULL);
	args = ft_split(cmd_part, ' ');
	if (!args || !args[0])
	{
		free_array(args);
		return (NULL);
	}
	new_block = malloc(sizeof(t_cmdblock));
	if (!new_block)
	{
		free_array(args);
		return (NULL);
	}
	new_block->exec = ft_strdup(args[0]);
	if (!new_block->exec)
	{
		free(new_block);
		free_array(args);
		return (NULL);
	}
	arg_count = cmd_count_args(cmd_part);
	new_block->args = (char **)malloc(sizeof(char *) * arg_count);
	if (!new_block->args)
	{
		free(new_block->exec);
		free(new_block);
		free_array(args);
		return (NULL);
	}
	i = 0;
	while (args[i + 1])
	{
		new_block->args[i] = ft_strdup(args[i + 1]);
		if (!new_block->args[i])
		{
			while (--i >= 0)
				free(new_block->args[i]);
			free(new_block->args);
			free(new_block->exec);
			free(new_block);
			free_array(args);
			return (NULL);
		}
		i++;
	}
	new_block->args[i] = NULL;
	free_array(args);
	new_block->next = NULL;
	return (new_block);
}

void	free_cmdblock_content(t_cmdblock *block)
{
	int	i;

	if (!block)
		return ;
	if (block->exec)
		free(block->exec);
	if (block->args)
	{
		i = 0;
		while (block->args[i])
			free(block->args[i++]);
		free(block->args);
	}
}

void	free_cmd_content(t_cmd *cmd)
{
	t_cmdblock	*current;
	t_cmdblock	*next;

	if (cmd->cmd_line)
		free(cmd->cmd_line);
	current = cmd->cmd;
	while (current)
	{
		next = current->next;
		free_cmdblock_content(current);
		free(current);
		current = next;
	}
	cmd->cmd_line = NULL;
	cmd->cmd = NULL;
}

void	cmd_init(char *rl, t_cmd *cmd, t_env *env)
{
	if (!rl || !cmd)
		return ;
	cmd->cmd = NULL;
	cmd->cmd_line = ft_strdup(rl);
	if (!cmd->cmd_line)
		return ;
	cmd_parser(rl, cmd, env);
}
