/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:28:29 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/17 14:55:37 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	print_export(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (!current->value)
			printf("declare -x %s\n", current->key);
		else
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
}

int	builtin_export(t_cmd *cmd, t_env *env)
{
	int		i;

	if (!cmd->cmd->args)
	{
		print_export(env);
		return (1);
	}
	i = -1;
	while (cmd->cmd->args[++i])
	{
		if (ft_strchr(cmd->cmd->args[i], '=') == NULL
			|| cmd->cmd->args[i][0] == '=')
			env_set(env, ft_strdup(cmd->cmd->args[i]), NULL, 0);
		else if (ft_strncmp(ft_strchr(cmd->cmd->args[i], '=') - 1, "+", 1))
			env_set(env, ft_strndup(cmd->cmd->args[i],
					ft_strchr(cmd->cmd->args[i], '=') - cmd->cmd->args[i]),
				ft_strdup(ft_strchr(cmd->cmd->args[i], '=') + 1), 0);
		else
			env_set(env, ft_strndup(cmd->cmd->args[i],
					ft_strchr(cmd->cmd->args[i], '=') - cmd->cmd->args[i] - 1),
				ft_strdup(ft_strchr(cmd->cmd->args[i], '=') + 1), 1);
	}
	return (1);
}

int	key_check(char *key, char *value, int plus)
{
	int		i;
	char	*s;

	s = ft_strdup(key);
	if (plus)
		s = ft_strjoin(s, "+");
	if (value)
		s = ft_strjoin(ft_strjoin(s, "="), value);
	i = -1;
	while (key[++i] != '\0')
	{
		if ((!ft_isalnum(key[i]) && i != 0 && key[i] != '_')
			|| (!ft_isalpha(key[i]) && i == 0 && key[i] != '_'))
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n", s);
			free(s);
			return (1);
		}
	}
	free(s);
	return (0);
}

/* int	builtin_export(t_cmd *cmd, t_env *env)
{
	int		i;
	char	*key;

	if (!cmd->cmd->args)
	{
		print_export(env);
		return (1);
	}
	i = -1;
	while (cmd->cmd->args[++i])
	{
		if (ft_strchr(cmd->cmd->args[i], '=') == NULL)
			env_set(env, ft_strdup(cmd->cmd->args[i]), NULL, 0);
		else if (ft_strncmp(ft_strchr(cmd->cmd->args[i], '=') - 1, "+", 1))
		{
			key = ft_strndup(cmd->cmd->args[i],
					ft_strchr(cmd->cmd->args[i], '=') - cmd->cmd->args[i]);
			env_set(env, key,
				ft_strdup(ft_strchr(cmd->cmd->args[i], '=') + 1), 0);
		}
		else
		{
			key = ft_strndup(cmd->cmd->args[i],
					ft_strchr(cmd->cmd->args[i], '=') - cmd->cmd->args[i] - 1);
			env_set(env, key,
				ft_strdup(ft_strchr(cmd->cmd->args[i], '=') + 1), 1);
		}
	}
	free(key);
	return (1);
} */
