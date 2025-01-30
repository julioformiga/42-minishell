/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:08:43 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/01/17 13:08:43 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator_start(char c)
{
	return (c == '|' || c == '<' || c == '>');
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

char	*extract_operator(char **rl)
{
	char	*token;

	if (**rl == '|' && *(*rl + 1) == '|')
	{
		token = ft_strdup("||");
		(*rl)++;
	}
	else if (**rl == '|')
		token = ft_strdup("|");
	else if (**rl == '<' && *(*rl + 1) == '<')
	{
		token = ft_strdup("<<");
		(*rl)++;
	}
	else if (**rl == '>' && *(*rl + 1) == '>')
	{
		token = ft_strdup(">>");
		(*rl)++;
	}
	else if (**rl == '<')
		token = ft_strdup("<");
	else
		token = ft_strdup(">");
	(*rl)++;
	return (token);
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
