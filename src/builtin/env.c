/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 21:19:48 by julio.formi       #+#    #+#             */
/*   Updated: 2025/02/02 17:29:42 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_update(t_env *env, char *key, char *value, int plus)
{
	char	*old_value;

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
		env = env->next;
	}
	return (1);
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
	return (ft_calloc(1, sizeof(char)));
}

int	env_set(t_env *env, char *key, char *value, int plus)
{
	t_env	*new;
	t_env	*last;

	if (env_key_check(key, value, plus, -1))//messaggio errore potrebbe essere chiamato anche da funzioni diverse da export
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
	new->value = NULL;
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
