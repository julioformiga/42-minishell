/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:17:32 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/09/16 16:17:32 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_free(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
	free(env);
}

int	ft_array_len(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
	array = NULL;
}

void	cmd_free(t_cmd *cmd)
{
	t_cmdblock	*block;
	t_cmdblock	*tmp;

	block = cmd->cmd;
	while (block)
	{
		tmp = block;
		block = block->next;
		free(tmp->exec);
		if (tmp->args)
			free_array(tmp->args);
		free(tmp);
	}
	free(cmd->cmd_line);
	free(cmd);
}
