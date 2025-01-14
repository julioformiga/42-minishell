/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 21:19:48 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/14 17:30:45 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	while (env != NULL)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

int	env_set(t_env *env, char *key, char *value)
{
	t_env	*new;
	t_env	*last;

	while (env != NULL)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			return (0);
		}
		last = env;
		env = env->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return (1);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	last->next = new;
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

int	builtin_env(t_cmd *cmd, t_env *env)
{
	t_env	*current;
	char	*line;

	(void)cmd;
	current = env;
	while (current)
	{
		line = ft_strjoin(current->key, "");
		if (current->value && ft_strncmp(current->value, "", 1) != 0)
		{
			line = ft_strjoin(line, "=");
			line = ft_strjoin(line, current->value);
		}
		line = ft_strjoin(line, "\n");
		ft_putstr_fd(line, STDOUT_FILENO);
		free(line);
		current = current->next;
	}
	return (1);
}
