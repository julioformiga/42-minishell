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

char	**env_to_array(t_env *env)
{
	char	**env_array;
	t_env	*current;
	char	*tmp;
	int		i;

	i = -1;
	current = env;
	while (i++, current)
		current = current->next;
	env_array = malloc(sizeof(char *) * (i + 1));
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

t_cmdblock	*get_first_block(t_cmdblock *block)
{
	t_cmdblock	*current;

	current = block;
	while (current && current->prev)
		current = current->prev;
	return (current);
}

static void	free_redirects(t_redirect *redirects)
{
	t_redirect	*current;
	t_redirect	*next;

	current = redirects;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		free(current);
		current = next;
	}
}

static void	free_cmdblock(t_cmdblock *block)
{
	t_cmdblock	*tmp;

	if (!block)
		return ;
	while (block)
	{
		tmp = block;
		block = block->next;
		if (tmp->exec)
			free(tmp->exec);
		if (tmp->args)
			free_array(tmp->args);
		if (tmp->redirects)
			free_redirects(tmp->redirects);
		free(tmp);
	}
}

void	free_cmd(t_cmd *cmd)
{
	t_cmdblock	*block;

	if (!cmd)
		return ;
	block = get_first_block(cmd->cmd);
	free_cmdblock(block);
	if (cmd->cmd_line)
		free(cmd->cmd_line);
	free(cmd);
}
