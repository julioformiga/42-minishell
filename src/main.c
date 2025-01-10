/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:34:41 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/08/30 13:34:41 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

char	*prompt(t_env *env)
{
	char	*prompt;
	char	*prompt_ini;
	char	*dir;
	char	*dir_home;
	char	*rl;

	dir = env_get(env, "PWD");
	dir_home = env_get(env, "HOME");
	if (ft_strncmp(dir, dir_home, ft_strlen(dir_home)) == 0)
		dir = ft_strjoin("~", dir + ft_strlen(dir_home));
	if (g_signal == 0)
		prompt_ini = ft_strjoin("\033[1;32m[minishell@42] ", dir);
	else if (g_signal == 1)
		prompt_ini = ft_strjoin("\033[1;31m[minishell@42] ", dir);
	else
		prompt_ini = ft_strjoin("\033[1;33m[minishell@42] ", dir);
	prompt = ft_strjoin(prompt_ini, " $>\033[0m ");
	free(prompt_ini);
	free(dir);
	rl = readline(prompt);
	free(prompt);
	return (rl);
}

static void	exec_process(t_cmd *cmd, t_env *env)
{
	char	*input;

	if (ft_atoi(env_get(env, "DEBUG")) == 1)
		cmd_debug(cmd);
	input = NULL;
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
	env_set(env, "DEBUG", ft_itoa(DEBUG));
	env_set(env, "a", "123");
	add_history("export DEBUG=1");
	add_history("ls | grep Makefile > result.txt");
	add_history("e\"ch\"o \"arg01> > $a |'nb'$a \" 'arg02> > $a$a' | cat");
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
