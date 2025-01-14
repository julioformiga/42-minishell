/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:28:29 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/14 17:46:06 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	print_export(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->value, "", 1) != 0)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
}

int	builtin_export(t_cmd *cmd, t_env *env)
{
	int		i;
	char	*key;

	if (!cmd->cmd->args[0])
	{
		print_export(env);
		return (1);
	}
	i = -1;
	while (cmd->cmd->args[++i])
	{
		if (ft_strchr(cmd->cmd->args[i], '='))
		{
			key = ft_strndup(cmd->cmd->args[i],
					ft_strchr(cmd->cmd->args[i], '=') - cmd->cmd->args[i]);
			env_set(env, key, ft_strdup(ft_strchr(cmd->cmd->args[i], '=') + 1));
		}
		else
		{
			key = ft_strdup(cmd->cmd->args[i]);
			env_set(env, key, "");
		}
		free(key);
	}
	return (1);
}
