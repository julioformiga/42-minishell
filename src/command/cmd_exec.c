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

static int	handle_builtin_cmd(t_cmd *cmdtmp, t_env *env,
							int *pipefd, int *prev_pipe)
{
	int	fd_output;
	int	result;

	fd_output = STDOUT_FILENO;
	result = cmd_exec_handle_redir(cmdtmp->cmd, pipefd, &fd_output);
	if (result)
		return (result);
	result = execute_builtin(cmdtmp, env, *prev_pipe, fd_output);
	if (fd_output != STDOUT_FILENO && fd_output != pipefd[1])
		close(fd_output);
	return (result);
}

static int	handle_external_cmd(t_cmd *cmdtmp, t_env *env,
							int *pipefd, int prev_pipe)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		if (cmdtmp->cmd->next)
		{
			cmd_exec_pipe_cmd(cmdtmp, env, prev_pipe, pipefd[1]);
			close(pipefd[0]);
		}
		else
			cmd_exec_pipe_cmd(cmdtmp, env, prev_pipe, STDOUT_FILENO);
	}
	return (0);
}

static int	execute_command(t_cmd *cmdtmp, t_env *env,
							int *pipefd, int *prev_pipe)
{
	int	result;

	result = 0;
	if (get_builtin(cmdtmp->cmd->exec))
		result = handle_builtin_cmd(cmdtmp, env, pipefd, prev_pipe);
	else if (handle_external_cmd(cmdtmp, env, pipefd, *prev_pipe))
		return (1);
	cmd_exec_setup_pipe(cmdtmp->cmd, pipefd, prev_pipe);
	return (result);
}

static int	update_command_position(t_cmd **cmdtmp)
{
	if (!(*cmdtmp)->cmd->next)
	{
		if ((*cmdtmp)->cmd->prev)
			(*cmdtmp)->cmd = (*cmdtmp)->cmd->prev;
		return (1);
	}
	(*cmdtmp)->cmd = (*cmdtmp)->cmd->next;
	return (0);
}

int	cmd_exec(t_cmd *cmd, t_env *env)
{
	int			pipefd[2];
	int			prev_pipe;
	int			result;
	t_cmd		*cmdtmp;

	result = 0;
	prev_pipe = STDIN_FILENO;
	pipefd[0] = -1;
	pipefd[1] = -1;
	cmdtmp = cmd;
	while (cmdtmp->cmd)
	{
		if (cmd_exec_pipe_check(cmdtmp, pipefd))
			return (1);
		result = execute_command(cmdtmp, env, pipefd, &prev_pipe);
		if (result == 1)
			return (g_signal = 1, 1);
		if (update_command_position(&cmdtmp))
			break ;
	}
	cmd_exec_pipe_wait_children(&result);
	return (WEXITSTATUS(result));
}
