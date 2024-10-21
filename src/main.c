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

void	input_process(char *rl, t_env *env)
{
	char	*input;

	if (rl)
	{
		input = ft_strtrim(rl, " \t\n\r");
		if (input)
		{
			add_history(input);
			cmd_exec(input, env);
			free(input);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	t_cmd	*cmd;
	char	*rl;

	env = env_init(envp);
	rl = NULL;
	cmd = malloc(sizeof(t_cmd));
	cmd_exec_inline(argc, argv, &env, cmd);
	while (g_signal != 2)
	{
		cmd = malloc(sizeof(t_cmd));
		rl = prompt(env);
		if (!builtin_exit(rl) || !rl)
			break ;
		cmd_init(cmd, rl);
		cmd_print(cmd);
		input_process(rl, env);
		free(cmd);
		free(rl);
	}
	env_free(env);
	return (g_signal);
}
