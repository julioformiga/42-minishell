/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:08:43 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/30 19:08:19 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator_start(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_operator	get_operator_type(const char *op)
{
	if (!op)
		return (OP_NONE);
	if (ft_strcmp(op, "|") == 0)
		return (OP_PIPE);
	if (ft_strcmp(op, "<") == 0)
		return (OP_REDIR_IN);
	if (ft_strcmp(op, ">") == 0)
		return (OP_REDIR_OUT);
	if (ft_strcmp(op, ">>") == 0)
		return (OP_REDIR_APPEND);
	if (ft_strcmp(op, "<<") == 0)
		return (OP_HEREDOC);
	return (OP_NONE);
}

static t_redirect	*create_redirect(t_operator type, char *file)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->op_type = type;
	redir->file = NULL;
	if (file)
		redir->file = ft_strdup(file);
	redir->next = NULL;
	return (redir);
}

int	add_redirect(t_cmdblock *block, t_operator type, char *file)
{
	t_redirect	*new_redir;
	t_redirect	*current;

	new_redir = create_redirect(type, file);
	if (!new_redir)
		return (0);
	if (!block->redirects)
		block->redirects = new_redir;
	else
	{
		current = block->redirects;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
	return (1);
}
