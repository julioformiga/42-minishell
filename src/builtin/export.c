/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:28:29 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/12/17 14:28:29 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	print_export(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
}

int	builtin_export(t_cmd *cmd, t_env *env)
{
	int		i;
	char	*key;
	char	*value;

	if (!cmd->cmd->args[0])
	{
		print_export(env);
		return (1);
	}
	i = -1;
	while (cmd->cmd->args[++i])
	{
		key = ft_strndup(cmd->cmd->args[i],
				ft_strchr(cmd->cmd->args[i], '=') - cmd->cmd->args[i]);
		value = ft_strdup(ft_strchr(cmd->cmd->args[i], '=') + 1);
		env_set(env, key, value);
		free(key);
		free(value);
	}
	return (1);
}
