/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_redirects_handle.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 07:12:57 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/03/26 07:12:57 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	cmd_exec_check_file(char *file)
{
	if (file[0] == '\0')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\
 `newline'\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int	open_output_file(char *file, int op_type)
{
	if (op_type == OP_REDIR_OUT)
		return (open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (op_type == OP_REDIR_APPEND)
		return (open(file, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (STDOUT_FILENO);
}

static int	handle_open_error(char *file)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(file, STDERR_FILENO);
	ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	return (-1);
}

static int	process_redirect(t_redirect *redir, int *fd_output,
							int *prev_fd, int *pipefd)
{
	if (cmd_exec_check_file(redir->file))
		return (g_signal = 2, 2);
	*prev_fd = *fd_output;
	*fd_output = open_output_file(redir->file, redir->op_type);
	if (*prev_fd != STDOUT_FILENO && *prev_fd != pipefd[1])
		close(*prev_fd);
	if (*fd_output == -1)
		return (handle_open_error(redir->file));
	return (0);
}

int	cmd_exec_handle_redir(t_cmdblock *block, int *pipefd, int *fd_output)
{
	t_redirect	*redir;
	int			prev_fd;
	int			result;

	*fd_output = STDOUT_FILENO;
	if (block->redirects)
	{
		if (block->next)
			close(pipefd[1]);
		redir = block->redirects;
		while (redir)
		{
			result = process_redirect(redir, fd_output, &prev_fd, pipefd);
			if (result != 0)
				return (result);
			redir = redir->next;
		}
	}
	else if (block->next)
		*fd_output = pipefd[1];
	return (0);
}
