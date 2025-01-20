/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:28:29 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/20 13:13:14 by scarlucc         ###   ########.fr       */
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

int	env_update(t_env *env, char *key, char *value, int plus)
{
	char	*old_value;
	t_env	*last;

	while (env != NULL)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			old_value = NULL;
			if (plus && env->value)
				old_value = ft_strdup(env->value);
			if (value)
				free(env->value);
			if (value && plus && old_value)
				env->value = ft_strjoin(old_value, value);
			else if (value)
				env->value = ft_strdup(value);
			free(old_value);
			return (0);
		}
		last = env;
		env = env->next;
	}
	return (1);
}

char	*build_string(char *err, char *value)
{
	char	*tmp;
	char	*new_s;

	if (value)
	{
		tmp = ft_strjoin(err, "=");
		if (!tmp)
		{
			free(err);
			return (NULL);
		}
		new_s = ft_strjoin(tmp, value);
		if (!new_s)
			return (NULL);
		free(tmp);
		free(err);
		err = new_s;
	}
	return (err);
}

int	env_key_check(char *key, char *value, int plus, int i)
{
	char	*err;
	char	*tmp;

	if (plus)
	{
		tmp = ft_strjoin(key, "+");
		if (!tmp)
			return (1);
	}
	else
		tmp = ft_strdup(key);
	err = build_string(tmp, value);
	if (err == NULL)
		return (1);
	while (key[++i] != '\0')
	{
		if ((!ft_isalnum(key[i]) && i != 0 && key[i] != '_')
			|| (!ft_isalpha(key[i]) && i == 0 && key[i] != '_'))
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n", err);
			return (free(err), 1);
		}
	}
	return (free(err), 0);
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
