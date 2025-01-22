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

void	cmd_free(t_cmd *cmd)
{
	t_cmdblock	*block;
	t_cmdblock	*tmp;
	t_redirect	*redir;

	block = cmd->cmd;
	while (block)
	{
		tmp = block;
		block = block->next;
		free(tmp->exec);
		if (tmp->args)
			free_array(tmp->args);
		if (tmp->redirects)
		{
			redir = tmp->redirects;
			free(redir->file);
			while (redir->next)
				free(redir->file);
		}
		free(tmp->redirects);
		free(tmp);
	}
	free(cmd->cmd_line);
	free(cmd);
}
