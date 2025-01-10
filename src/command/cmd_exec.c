/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 01:42:07 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/09/23 01:42:07 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_piped_command(t_cmd *cmd, t_env *env,
									int input_fd, int output_fd)
{
	char			*full_path;
	char			**args;
	char			**env_array;

	if (cmd_setup(cmd, env, &args, &full_path) != 0)
		exit(1);
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	env_array = env_to_array(env);
	execve(full_path, args, env_array);
	free_array(env_array);
	free_array(args);
	free(full_path);
	perror("execve");
	exit(1);
}

void	cmd_exec_inline(int argc, char **argv, t_env *env, t_cmd *cmd)
{
	extern int	g_signal;

	if (argc == 3 && argv[1] && ft_strncmp(argv[1], "-c", 3) == 0)
	{
		cmd_init(argv[2], cmd, env);
		g_signal = cmd_exec(cmd, env);
		cmd_free(cmd);
		env_free(env);
		exit(g_signal);
	}
	else if (argc > 1)
	{
		printf("Usage:\n./minishell\nOR\n./minishell -c \"command\"\n");
		cmd_free(cmd);
		g_signal = 2;
	}
}

int	cmd_exec(t_cmd *cmd, t_env *env)
{
	int		pipefd[2];
	int		prev_pipe;
	pid_t	pid;
	int		status;
	t_cmd	*current;

	prev_pipe = STDIN_FILENO;
	current = cmd;
	while (current->cmd)
	{
		if (current->cmd->next && pipe(pipefd) == -1)
		{
			perror("pipe");
			return (1);
		}
		if (get_builtin(current->cmd->exec))
		{
			if (current->cmd->next)
			{
				status = execute_builtin(current, env, prev_pipe, pipefd[1]);
				close(pipefd[1]);
			}
			else
				status = execute_builtin(current, env, prev_pipe,
						STDOUT_FILENO);
		}
		else
		{
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				return (1);
			}
			else if (pid == 0)
			{
				if (current->cmd->next)
				{
					execute_piped_command(current, env, prev_pipe, pipefd[1]);
					close(pipefd[0]);
				}
				else
					execute_piped_command(current, env, prev_pipe,
						STDOUT_FILENO);
			}
		}
		if (prev_pipe != STDIN_FILENO)
			close(prev_pipe);
		if (current->cmd->next)
		{
			close(pipefd[1]);
			prev_pipe = pipefd[0];
		}
		current->cmd = current->cmd->next;
	}
	while (wait(&status) > 0)
		;
	return (WEXITSTATUS(status));
}
