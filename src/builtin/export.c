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

static	void	export_print(t_env *env)
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

static int	handle_no_value_export(t_env *env, char *arg)
{
	char	*key;
	int		result;

	key = ft_strdup(arg);
	if (!key)
		return (1);
	result = env_set(env, key, NULL, 0);
	free(key);
	return (result);
}

static int	handle_value_export(t_env *env, char *arg, int has_plus)
{
	char	*key;
	char	*value;
	char	*equal_pos;
	int		key_len;
	int		result;

	equal_pos = ft_strchr(arg, '=');
	key_len = equal_pos - arg;
	if (has_plus)
		key_len--;
	key = ft_strndup(arg, key_len);
	if (!key)
		return (1);
	value = ft_strdup(equal_pos + 1);
	if (!value)
		return (free(key), 1);
	result = env_set(env, key, value, has_plus);
	free(key);
	free(value);
	return (result);
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

int	builtin_export(t_cmd *cmd, t_env *env)
{
	char	**args;
	int		i;
	int		has_plus;

	args = cmd->cmd->args;
	if (!args)
		return (export_print(env), 0);
	i = -1;
	while (args[++i])
	{
		if (ft_strchr(args[i], '=') == NULL || args[i][0] == '=')
		{
			if (handle_no_value_export(env, args[i]))
				return (1);
		}
		else
		{
			has_plus = !ft_strncmp(ft_strchr(args[i], '=') - 1, "+", 1);
			if (handle_value_export(env, args[i], has_plus))
				return (1);
		}
	}
	return (0);
}
