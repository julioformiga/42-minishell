/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:11:10 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/11/18 15:11:10 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_cmdblock	*get_first_block(t_cmdblock *block)
{
	t_cmdblock	*current;

	current = block;
	while (current && current->prev)
		current = current->prev;
	return (current);
}

static void	free_redirects(t_redirect *redirects)
{
	t_redirect	*current;
	t_redirect	*next;

	current = redirects;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		free(current);
		current = next;
	}
}

static void	free_cmdblock(t_cmdblock *block)
{
	t_cmdblock	*tmp;

	if (!block)
		return ;
	while (block)
	{
		tmp = block;
		block = block->next;
		if (tmp->exec)
			free(tmp->exec);
		if (tmp->args)
			free_array(tmp->args);
		if (tmp->redirects)
			free_redirects(tmp->redirects);
		free(tmp);
	}
}

void	free_cmd(t_cmd *cmd)
{
	t_cmdblock	*block;

	if (!cmd)
		return ;
	block = get_first_block(cmd->cmd);
	free_cmdblock(block);
	if (cmd->cmd_line)
		free(cmd->cmd_line);
	free(cmd);
}
