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
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

static int	setup_redirections(t_redirect *redirects)
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

static void	execute_piped_command(t_cmd *cmd, t_env *env,
									int input_fd, int output_fd)
{
	char			*full_path;
	char			**args;
	char			**env_array;

	if (cmd_setup(cmd, env, &args, &full_path) != 0)
		exit(g_signal);
	if (cmd->cmd->redirects && setup_redirections(cmd->cmd->redirects) == -1)
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
	free(full_path);
	perror("execve error");
	exit(1);
}

void	cmd_exec_inline(int argc, char **argv, t_env *env, t_cmd *cmd)
{
	if (argc == 3 && argv[1] && ft_strncmp(argv[1], "-c", 3) == 0)
	{
		cmd_init(argv[2], cmd, env);
		cmd_exec(cmd, env);
		cmd_free(cmd);
		env_free(env);
		exit(g_signal);
	}
	else if (argc > 1)
	{
		printf("Usage:\n./minishell\nOR\n./minishell -c \"command\"\n");
		g_signal = 2;
	}
}

int	cmd_exec(t_cmd *cmd, t_env *env)
{
	int			pipefd[2];
	int			prev_pipe;
	int			result;
	pid_t		pid;
	t_cmdblock	*current;
	t_redirect	*redir;
	int			final_output_fd;

	result = 0;
	prev_pipe = STDIN_FILENO;
	current = cmd->cmd;
	while (current)
	{
		if (current->next && pipe(pipefd) == -1)
		{
			perror("pipe");
			return (1);
		}
		if (get_builtin(current->exec))
		{
			final_output_fd = STDOUT_FILENO;
			if (current->redirects)
			{
				if (current->next)
					close(pipefd[1]);
				redir = current->redirects;
				while (redir)
				{
					if (redir->op_type == OP_REDIR_OUT)
					{
						final_output_fd = open(redir->file,
								O_WRONLY | O_CREAT | O_TRUNC, 0644);
					}
					else if (redir->op_type == OP_REDIR_APPEND)
					{
						final_output_fd = open(redir->file,
								O_WRONLY | O_CREAT | O_APPEND, 0644);
					}
					redir = redir->next;
				}
			}
			else if (current->next)
				final_output_fd = pipefd[1];
			result = execute_builtin(cmd, env,
					prev_pipe, final_output_fd);
			if (final_output_fd != STDOUT_FILENO
				&& final_output_fd != pipefd[1])
				close(final_output_fd);
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
				if (current->next)
				{
					execute_piped_command(cmd, env, prev_pipe, pipefd[1]);
					close(pipefd[0]);
				}
				else
					execute_piped_command(cmd, env, prev_pipe,
						STDOUT_FILENO);
			}
		}
		if (prev_pipe != STDIN_FILENO)
			close(prev_pipe);
		if (current->next)
		{
			close(pipefd[1]);
			prev_pipe = pipefd[0];
		}
		current = current->next;
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
