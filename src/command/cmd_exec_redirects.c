/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:39:56 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/01/24 16:39:56 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_exec_handle_redirect(t_cmdblock *block, int *pipefd, int *fd_output)
{
	t_redirect	*redir;

	*fd_output = STDOUT_FILENO;
	if (block->redirects)
	{
		if (block->next)
			close(pipefd[1]);
		redir = block->redirects;
		while (redir)
		{
			if (redir->op_type == OP_REDIR_OUT)
				*fd_output = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC,
						0644);
			else if (redir->op_type == OP_REDIR_APPEND)
				*fd_output = open(redir->file, O_WRONLY | O_CREAT | O_APPEND,
						0644);
			redir = redir->next;
		}
	}
	else if (block->next)
		*fd_output = pipefd[1];
}

static int	setup_input_redirect(t_redirect *current, int *fd)
{
	*fd = open(current->file, O_RDONLY);
	if (*fd == -1)
		return (-1);
	dup2(*fd, STDIN_FILENO);
	close(*fd);
	return (0);
}

static int	setup_output_redirect(t_redirect *current, int *fd)
{
	int	flags;

	if (current->op_type == OP_REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	*fd = open(current->file, flags, 0644);
	if (*fd == -1)
		return (-1);
	dup2(*fd, STDOUT_FILENO);
	close(*fd);
	return (0);
}

int	cmd_exec_setup_redirect(t_redirect *redirects)
{
	t_redirect	*current;
	int			fd;
	int			heredoc_fd;

	if (cmd_exec_setup_heredoc(redirects, &heredoc_fd) == -1)
		return (-1);
	current = redirects;
	while (current)
	{
		if (current->op_type == OP_REDIR_IN
			&& setup_input_redirect(current, &fd))
			return (-1);
		else if (current->op_type == OP_HEREDOC)
		{
			dup2(heredoc_fd, STDIN_FILENO);
			close(heredoc_fd);
		}
		else if ((current->op_type == OP_REDIR_OUT
				|| current->op_type == OP_REDIR_APPEND)
			&& setup_output_redirect(current, &fd))
			return (-1);
		current = current->next;
	}
	return (0);
}
