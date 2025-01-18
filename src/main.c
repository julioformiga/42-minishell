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
	if (ft_strcmp(dir, dir_home) == 0)
		dir = ft_strjoin("~", dir + ft_strlen(dir_home));
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
	if (cmd->cmd->exec)
	{
		input = ft_strtrim(cmd->cmd->exec, " \t\n\r");
		if (input)
		{
			free(input);
			add_history(cmd->cmd_line);
			cmd_exec(cmd, env);
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

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	t_cmd	*cmd;
	char	*rl;

	setup_signals();
	env = env_init(envp);
	rl = NULL;
	cmd = malloc(sizeof(t_cmd));
	env_set(env, "DEBUG", "1", 0);
	env_set(env, "a", "123", 0);
	env_set(env, "c", "cho", 0);
	add_history("export DEBUG=1");
	add_history("ls | grep Makefile > result.txt");
	add_history("e$c \"arg01>     > $a |'nb'$a   \" a$USER 'arg02> > $a$a' | cat >$USER result.txt");
	add_history("cat << EOF | tr a-z A-Z > uppercase.txt");
	add_history("echo $$USER");
	add_history("echo $\"USER\"");
	add_history("> output.txt ls");
	add_history("> output.txt echo asd");
	add_history("< output.txt cat");
	cmd_exec_inline(argc, argv, env, cmd);
	free(cmd);
	while (g_signal != 2)
	{
		cmd = malloc(sizeof(t_cmd));
		rl = prompt(env);
		if (!rl)
		{
			printf("exit\n");
			free(cmd);
			break ;
		}
		if (!ft_check_only_spaces(rl))
		{
			free(cmd);
			free(rl);
			continue ;
		}
		cmd_init(rl, cmd, env);
		free(rl);
		exec_process(cmd, env);
		cmd_free(cmd);
	}
	env_free(env);
	return (g_signal);
}
