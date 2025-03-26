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

static int	handle_cmd_builtin(t_cmd *cmdtmp, t_env *env,
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

static int	handle_cmd_external(t_cmd *cmdtmp, t_env *env,
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
		if (prev_pipe != STDIN_FILENO)
			dup2(prev_pipe, STDIN_FILENO);
		if (cmdtmp->cmd->next)
		{
			cmd_exec_pipe_cmd(cmdtmp, env, prev_pipe, pipefd[1]);
			close(pipefd[0]);
		}
		else
			cmd_exec_pipe_cmd(cmdtmp, env, prev_pipe, STDOUT_FILENO);
		if (prev_pipe != STDIN_FILENO)
			close(prev_pipe);
	}
	return (0);
}

static int	handle_cmd_null(t_cmd *cmdtmp, int *pipefd, int *prev_pipe)
{
	int		fd_output;
	int		result;
	int		heredoc_fd;

	fd_output = STDOUT_FILENO;
	result = cmd_exec_handle_redir(cmdtmp->cmd, pipefd, &fd_output);
	if (result)
		return (result);
	if (cmdtmp->cmd->redirects && fd_output == STDOUT_FILENO)
	{
		if (cmd_exec_setup_heredoc(cmdtmp->cmd->redirects, &heredoc_fd))
			return (1);
	}
	else
	{
		if (fd_output != STDOUT_FILENO && fd_output != pipefd[1])
			close(fd_output);
		if (cmdtmp->cmd->next)
		{
			close(pipefd[1]);
			*prev_pipe = pipefd[0];
		}
	}
	return (0);
}

static int	execute_command(t_cmd *cmdtmp, t_env *env,
							int *pipefd, int *prev_pipe)
{
	int	result;

	result = 0;
	if (!cmdtmp->cmd->exec)
		result = handle_cmd_null(cmdtmp, pipefd, prev_pipe);
	else if (get_builtin(cmdtmp->cmd->exec))
		result = handle_cmd_builtin(cmdtmp, env, pipefd, prev_pipe);
	else if (handle_cmd_external(cmdtmp, env, pipefd, *prev_pipe))
		return (1);
	cmd_exec_setup_pipe(cmdtmp->cmd, pipefd, prev_pipe);
	return (result);
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
		if (cmd_exec_update_position_cmd(&cmdtmp))
			break ;
	}
	cmd_exec_pipe_wait_children(&result);
	return (WEXITSTATUS(result));
}
