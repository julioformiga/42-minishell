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

void	cmd_exec_pipe_close_fds(void)
{
	int	fd;

	fd = 2;
	while (fd++, fd < 256)
		if (fcntl(fd, F_GETFD) != -1)
			close(fd);
}

void	cmd_exec_pipe_wait_children(int *result)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				g_signal = 130;
			else if (WTERMSIG(status) == SIGPIPE)
				g_signal = 0;
			else
				g_signal = WTERMSIG(status) + 128;
		}
		else if (WIFEXITED(status))
			g_signal = WEXITSTATUS(status);
		*result = status;
		pid = waitpid(-1, result, 0);
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
	{
		close(*prev_pipe);
		*prev_pipe = STDIN_FILENO;
	}
	if (block->next)
	{
		close(fd[1]);
		*prev_pipe = fd[0];
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
}
