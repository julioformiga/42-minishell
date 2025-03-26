/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_pipe_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 07:25:02 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/03/26 07:25:02 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_pipe_fds(t_cmdblock *cmd, int input_fd, int output_fd)
{
	signal(SIGQUIT, SIG_DFL);
	if (input_fd != STDIN_FILENO)
	{
		if (!cmd->redirects)
		{
			if (dup2(input_fd, STDIN_FILENO) == -1)
				perror("dup2");
		}
		if (input_fd > 2)
			close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		if (!cmd->redirects)
		{
			if (dup2(output_fd, STDOUT_FILENO) == -1)
				perror("dup2");
		}
		if (output_fd > 2)
			close(output_fd);
	}
	if (cmd->next)
		cmd_exec_pipe_close_fds();
}

static void	exec_cmd_cleanup(t_cmd *cmd, t_env *env, char **args,
	char *full_path)
{
	cmd_exec_pipe_close_fds();
	free_array(args);
	free_cmd(cmd);
	env_free(env);
	free(full_path);
}

void	cmd_exec_with_path(t_cmd *cmd, t_env *env, char **args, char *full_path)
{
	char	**env_array;
	char	*msg_error;

	msg_error = NULL;
	env_array = env_to_array(env);
	execve(full_path, args, env_array);
	free_array(env_array);
	msg_error = ft_strdup("execve error");
	exec_cmd_cleanup(cmd, env, args, full_path);
	perror(msg_error);
	free(msg_error);
	exit(1);
}

void	cmd_exec_pipe_cmd(t_cmd *cmd, t_env *env, int infd, int outfd)
{
	char	*full_path;
	char	**args;
	char	*msg_error;

	msg_error = NULL;
	if (cmd_setup(cmd, env, &args, &full_path) != 0)
		exit(g_signal);
	if (cmd->cmd->redirects && cmd_exec_setup_redir(cmd->cmd->redirects) == -1)
		msg_error = ft_strdup("redirect error");
	if (!msg_error)
	{
		setup_pipe_fds(cmd->cmd, infd, outfd);
		cmd_exec_with_path(cmd, env, args, full_path);
	}
	exec_cmd_cleanup(cmd, env, args, full_path);
	perror(msg_error);
	free(msg_error);
	exit(1);
}
