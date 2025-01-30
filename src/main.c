/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:34:41 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/17 14:01:06 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

char	*prompt(t_env *env)
{
	char	*prompt_ini;
	char	*prompt;
	char	*dir;
	char	*dir_home;
	char	*rl;

	dir = env_get(env, "PWD");
	dir_home = env_get(env, "HOME");
	prompt_ini = ft_strjoin("[minishell@42] ", dir);
	prompt = ft_strjoin(prompt_ini, " $> ");
	free(prompt_ini);
	free(dir);
	free(dir_home);
	rl = readline(prompt);
	free(prompt);
	return (rl);
}

static void	exec_process(t_cmd *cmd, t_env *env)
{
	char	*input;
	char	*debug;

	debug = env_get(env, "DEBUG");
	if (ft_atoi(debug) == 1)
		cmd_debug(cmd);
	free(debug);
	if (cmd->cmd_line)
	{
		input = ft_strtrim(cmd->cmd_line, " \t\n\r");
		if (input)
		{
			free(input);
			add_history(cmd->cmd_line);
			save_history(cmd->cmd_line, env);
			cmd_exec(cmd, env);
			free_cmd(cmd);
		}
	}
}

static int	ft_check_only_spaces(char *str)
{
	while (*str)
	{
		if (!isspace(*str))
			return (1);
		str++;
	}
	return (0);
}

static void	main_init(int argc, char **argv, t_env *env)
{
	t_cmd	*cmd;

	setup_signals();
	cmd = malloc(sizeof(t_cmd));
	env_set(env, "DEBUG", "0", 0);
	env_set(env, "a", "123", 0);
	env_set(env, "c", "cho", 0);
	load_history(env);
	cmd_exec_inline(argc, argv, env, cmd);
	free(cmd);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	t_cmd	*cmd;
	char	*rl;

	env = env_init(envp);
	main_init(argc, argv, env);
	while (1)
	{
		rl = prompt(env);
		if (!rl)
		{
			printf("exit\n");
			break ;
		}
		if (!ft_check_only_spaces(rl))
			continue ;
		cmd = malloc(sizeof(t_cmd));
		cmd_init(rl, cmd, env);
		free(rl);
		exec_process(cmd, env);
	}
	env_free(env);
	return (g_signal);
}
