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
		return (g_signal);
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
