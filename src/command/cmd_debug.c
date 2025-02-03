/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_debug.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 23:56:22 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/01/21 23:56:22 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_redirect_op(int op_type)
{
	if (op_type == OP_PIPE)
		return ("|");
	if (op_type == OP_REDIR_IN)
		return ("<");
	if (op_type == OP_REDIR_OUT)
		return (">");
	if (op_type == OP_REDIR_APPEND)
		return (">>");
	if (op_type == OP_HEREDOC)
		return ("<<");
	return ("unknown");
}

static void	cmd_debug_redirects(t_redirect *redir)
{
	while (redir)
	{
		printf("|\t\tRedirect: %s\n", get_redirect_op(redir->op_type));
		printf("|\t\t\tFile: %s\n", redir->file);
		redir = redir->next;
	}
}

void	cmd_debug(t_cmd *cmd)
{
	t_cmdblock	*block;
	int			i;
	int			n;

	block = cmd->cmd;
	printf("\n+-----------------------------------------------------------+\n");
	printf("| Full command: %s\n", cmd->cmd_line);
	i = 0;
	while (i++, block)
	{
		n = -1;
		printf("|\tCommand #%d: %s\n", i, block->exec);
		if (block->fd_in != -1 || block->fd_out != -1)
			printf("|\t\tFD0[%d] - FD1[%d]\n", block->fd_in, block->fd_out);
		if (block->args)
			while (block->args[++n])
				printf("|\t\tArg #%d: %s\n", n + 1, block->args[n]);
		if (block->redirects)
			cmd_debug_redirects(block->redirects);
		if (block->op_type)
			printf("|\t\tOperator: %s\n", get_redirect_op(block->op_type));
		block = block->next;
	}
	printf("+-----------------------------------------------------------+\n");
}
