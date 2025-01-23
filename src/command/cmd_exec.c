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

void	cmd_exec_inline(int argc, char **argv, t_env *env, t_cmd *cmd)
{
	if (argc == 3 && argv[1] && ft_strncmp(argv[1], "-c", 3) == 0)
	{
		cmd_init(argv[2], cmd, env);
		cmd_exec(cmd, env);
		free_cmd(cmd);
		env_free(env);
		exit(g_signal);
	}
	else if (argc > 1)
	{
		printf("Usage:\n./minishell\nOR\n./minishell -c \"command\"\n");
		g_signal = 2;
	}
}

static void handle_redirections(t_cmdblock *block, int *pipefd, int *fd_output)
{
	t_redirect *redir;

	*fd_output = STDOUT_FILENO;
	if (block->redirects)
	{
		if (block->next)
			close(pipefd[1]);
		redir = block->redirects;
		while (redir)
		{
			if (redir->op_type == OP_REDIR_OUT)
				*fd_output = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (redir->op_type == OP_REDIR_APPEND)
				*fd_output = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			redir = redir->next;
		}
	}
	else if (block->next)
		*fd_output = pipefd[1];
}

static void handle_pipe_setup(t_cmdblock *block, int *pipefd, int *prev_pipe)
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
			handle_redirections(cmdtmp->cmd, pipefd, &fd_output);
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
				{
					exec_piped_cmd(cmdtmp, env, prev_pipe, STDOUT_FILENO);
				}
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
