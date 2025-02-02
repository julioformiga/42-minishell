/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:12:09 by julio.formi       #+#    #+#             */
/*   Updated: 2025/02/02 17:10:34 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset_key_check(char *key)
{
	int		i;

	i = -1;
	while (key[++i] != '\0')
	{
		if ((!ft_isalnum(key[i]) && i != 0 && key[i] != '_')
			|| (!ft_isalpha(key[i]) && i == 0 && key[i] != '_'))
		{
			ft_printf("minishell: unset: `%s': not a valid identifier\n", key);
			return (1);
		}
	}
	return (0);
}

int	env_unset(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(current->key)) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

int	builtin_unset(t_cmd *cmd, t_env *env)
{
	int	i;
	int	sig_ret;

	i = -1;
	sig_ret = 0;
	if (cmd->cmd->args)
	{
		while (cmd->cmd->args[++i])
		{
			if (!unset_key_check(cmd->cmd->args[i]))
				env_unset(&env, cmd->cmd->args[i]);
			else
				sig_ret = 1;
		}
	}
	return (sig_ret);
}
