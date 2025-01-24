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

static void	handle_pipe_setup(t_cmdblock *block, int *pipefd, int *prev_pipe)
{
	if (*prev_pipe != STDIN_FILENO)
		close(*prev_pipe);
	if (block->next)
	{
		close(pipefd[1]);
		*prev_pipe = pipefd[0];
	}
}

static void	exec_piped_cmd(t_cmd *cmd, t_env *env,
									int input_fd, int output_fd)
{
	char			*full_path;
	char			**args;
	char			**env_array;

	if (cmd_setup(cmd, env, &args, &full_path) != 0)
		exit(g_signal);
	if (cmd->cmd->redirects
		&& cmd_exec_setup_redirect(cmd->cmd->redirects) == -1)
	{
		perror("redirect error");
		exit(1);
	}
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
	free_cmd(cmd);
	env_free(env);
	free(full_path);
	perror("execve error");
	exit(1);
}

int	cmd_exec(t_cmd *cmd, t_env *env)
{
	int			pipefd[2];
	int			prev_pipe;
	int			result;
	pid_t		pid;
	t_cmd		*cmdtmp;
	int			fd_output;

	result = 0;
	prev_pipe = STDIN_FILENO;
	fd_output = STDOUT_FILENO;
	pipefd[0] = -1;
	pipefd[1] = -1;
	cmdtmp = cmd;
	while (cmdtmp->cmd)
	{
		if (cmdtmp->cmd->next && pipe(pipefd) == -1)
		{
			perror("pipe");
			return (1);
		}
		if (get_builtin(cmdtmp->cmd->exec))
		{
			cmd_exec_handle_redirect(cmdtmp->cmd, pipefd, &fd_output);
			result = execute_builtin(cmdtmp, env, prev_pipe, fd_output);
			if (fd_output != STDOUT_FILENO && fd_output != pipefd[1])
				close(fd_output);
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
				if (cmdtmp->cmd->next)
				{
					exec_piped_cmd(cmdtmp, env, prev_pipe, pipefd[1]);
					close(pipefd[0]);
				}
				else
					exec_piped_cmd(cmdtmp, env, prev_pipe, STDOUT_FILENO);
			}
		}
		handle_pipe_setup(cmdtmp->cmd, pipefd, &prev_pipe);
		if (!cmdtmp->cmd->next)
		{
			if (cmdtmp->cmd->prev)
				cmdtmp->cmd = cmdtmp->cmd->prev;
			break ;
		}
		cmdtmp->cmd = cmdtmp->cmd->next;
	}
	while (wait(&result) > 0)
	{
		if (WIFEXITED(result))
			g_signal = WEXITSTATUS(result);
		else if (WIFSIGNALED(result))
			g_signal = WTERMSIG(result) + 128;
	}
	return (WEXITSTATUS(result));
}
