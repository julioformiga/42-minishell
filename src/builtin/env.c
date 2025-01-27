/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 21:19:48 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/20 12:29:50 by scarlucc         ###   ########.fr       */
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

t_env	*env_init(char **envp)
{
	t_env	*env;
	t_env	*tmp;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		tmp = malloc(sizeof(t_env));
		tmp->key = ft_strndup(envp[i], ft_strchr(envp[i], '=') - envp[i]);
		tmp->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		if (!tmp->key || !tmp->value)
		{
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return (NULL);
		}
		tmp->next = env;
		env = tmp;
		i++;
	}
	return (env);
}

char	*env_get(t_env *env, char *key)
{
	char	*value;

	if (!env || !key)
		return (NULL);
	while (env != NULL)
	{
		if (ft_strcmp(env->key, key) == 0 && env->value)
		{
			value = ft_strdup(env->value);
			if (!value)
				return (NULL);
			return (value);
		}
		env = env->next;
	}
	return ("");
}

int	env_set(t_env *env, char *key, char *value, int plus)
{
	t_env	*new;
	t_env	*last;

	if (env_key_check(key, value, plus, -1))
		return (1);
	if (!env_update(env, key, value, plus))
		return (0);
	while (env != NULL)
	{
		last = env;
		env = env->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return (1);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	new->next = NULL;
	last->next = new;
	return (0);
}

int	builtin_env(t_cmd *cmd, t_env *env)
{
	t_env	*current;

	(void)cmd;
	current = env;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		current = current->next;
	}
	return (0);
}
