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

static int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;
	size_t	delim_len;

	if (pipe(pipe_fd) == -1)
		return (-1);
	delim_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("pipe heredoc> ");
		if (!line)
		{
			close(pipe_fd[1]);
			return (-1);
		}
		if (ft_strlen(line) == delim_len && ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	return (close(pipe_fd[1]), pipe_fd[0]);
}

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

int	cmd_exec_setup_redirect(t_redirect *redirects)
{
	t_redirect	*current;
	int			fd;
	int			heredoc_fd;

	heredoc_fd = -1;
	current = redirects;
	while (current)
	{
		if (current->op_type == OP_HEREDOC)
		{
			heredoc_fd = handle_heredoc(current->file);
			if (heredoc_fd == -1)
				return (-1);
		}
		current = current->next;
	}
	current = redirects;
	while (current)
	{
		if (current->op_type == OP_REDIR_IN)
		{
			fd = open(current->file, O_RDONLY);
			if (fd == -1)
				return (-1);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (current->op_type == OP_HEREDOC)
		{
			dup2(heredoc_fd, STDIN_FILENO);
			close(heredoc_fd);
		}
		else if (current->op_type == OP_REDIR_OUT)
		{
			fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (-1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (current->op_type == OP_REDIR_APPEND)
		{
			fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (-1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		current = current->next;
	}
	return (0);
}
