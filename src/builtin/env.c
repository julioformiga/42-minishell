/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 21:19:48 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/11/28 21:19:48 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
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
		line = ft_strjoin(current->key, "=");
		line = ft_strjoin_free(line, current->value);
		line = ft_strjoin_free(line, "\n");
		ft_putstr_fd(line, STDOUT_FILENO);
		free(line);
		current = current->next;
	}
	return (1);
}
