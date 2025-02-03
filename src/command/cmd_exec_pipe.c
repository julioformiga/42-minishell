/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 17:28:02 by julio.formi       #+#    #+#             */
/*   Updated: 2025/02/03 12:14:32 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_pipe_fds(int input_fd, int output_fd)
{
	signal(SIGQUIT, SIG_DFL);
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
}

void	cmd_exec_pipe_cmd(t_cmd *cmd, t_env *env, int infd, int outfd)
{
	char	*full_path;
	char	**args;
	char	**env_array;
	char	*msg_error;

	msg_error = NULL;
	if (cmd_setup(cmd, env, &args, &full_path) != 0)
		exit(g_signal);
	if (cmd->cmd->redirects && cmd_exec_setup_redir(cmd->cmd->redirects) == -1)
		msg_error = ft_strdup("redirect error");
	if (!msg_error)
	{
		setup_pipe_fds(infd, outfd);
		env_array = env_to_array(env);
		execve(full_path, args, env_array);
		free_array(env_array);
		msg_error = ft_strdup("execve error");
	}
	free_array(args);
	free_cmd(cmd);
	env_free(env);
	free(full_path);
	perror(msg_error);
	free(msg_error);
	exit(1);
}

void	cmd_exec_pipe_wait_children(int *result)
{
	pid_t	pid;
	int		last_running_pid;

	last_running_pid = 0;
	pid = wait(result);
	while (pid > 0)
	{
		pid = wait(result);
		if (WIFSIGNALED(*result) && WTERMSIG(*result) == SIGINT)
		{
			if (last_running_pid == 0 || pid == last_running_pid)
				g_signal = 130;
			else
				g_signal = 0;
		}
		else if (WIFEXITED(*result))
			g_signal = WEXITSTATUS(*result);
		else if (WIFSIGNALED(*result))
			g_signal = WTERMSIG(*result) + 128;
		last_running_pid = pid;
	}
}

int	cmd_exec_pipe_check(t_cmd *cmdtmp, int *pipefd)
{
	if (!cmdtmp->cmd->exec && !cmdtmp->cmd->redirects)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		g_signal = 2;
		return (2);
	}
	if (cmdtmp->cmd->next && pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

void	cmd_exec_setup_pipe(t_cmdblock *block, int *fd, int *prev_pipe)
{
	if (*prev_pipe != STDIN_FILENO)
		close(*prev_pipe);
	if (block->next)
	{
		close(fd[1]);
		*prev_pipe = fd[0];
	}
}
