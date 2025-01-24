/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 17:12:25 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/01/24 17:12:25 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_exec_handle_heredoc(char *delimiter)
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

int	cmd_exec_setup_heredoc(t_redirect *redirects, int *heredoc_fd)
{
	t_redirect	*current;

	*heredoc_fd = -1;
	current = redirects;
	while (current)
	{
		if (current->op_type == OP_HEREDOC)
		{
			*heredoc_fd = cmd_exec_handle_heredoc(current->file);
			if (*heredoc_fd == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}
