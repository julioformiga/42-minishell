/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 19:25:53 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/12 18:28:38 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static t_builtin	*get_builtins(void)
{
	static t_builtin	builtins[] = {
	{"echo", builtin_echo},
	{"cd", builtin_cd},
	{"exit", builtin_exit},
	{"e", builtin_exit},
	{"env", builtin_env},
	{"export", builtin_export},
	{"unset", builtin_unset},
	{"pwd", builtin_pwd},
	{NULL, NULL}
	};

	return (builtins);
}

t_builtin_fn	get_builtin(char *cmd_name)
{
	t_builtin	*builtins;
	int			i;

	builtins = get_builtins();
	i = -1;
	while (builtins[++i].name)
	{
		if (ft_strcmp(cmd_name, builtins[i].name) == 0)
			return (builtins[i].fn);
	}
	return (NULL);
}

int	execute_builtin(t_cmd *cmd, t_env *env, int input_fd, int output_fd)
{
	int	stdout_backup;
	int	stdin_backup;
	int	result;

	stdout_backup = dup(STDOUT_FILENO);
	stdin_backup = dup(STDIN_FILENO);
	if (input_fd != STDIN_FILENO)
		dup2(input_fd, STDIN_FILENO);
	if (output_fd != STDOUT_FILENO)
		dup2(output_fd, STDOUT_FILENO);
	result = get_builtin(cmd->cmd->exec)(cmd, env);
	dup2(stdout_backup, STDOUT_FILENO);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdout_backup);
	close(stdin_backup);
	return (g_signal = result, result);
}
