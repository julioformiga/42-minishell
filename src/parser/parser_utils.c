/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:49:13 by scarlucc          #+#    #+#             */
/*   Updated: 2025/02/03 09:32:51 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**init_matrix_tokens(int tok_count)
{
	char	**tokens;

	tokens = malloc(sizeof(char *) * (tok_count + 1));
	if (!tokens)
		return (NULL);
	tokens[0] = ft_strdup("");
	return (tokens);
}

int	check_previuos_op(char *op, char **tokens, int i)
{
	if (i > 0)
	{
		if (is_operator_start(tokens[i - 1][0]) && is_operator_start(op[0]))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `",
				STDERR_FILENO);
			ft_putstr_fd(op, STDERR_FILENO);
			ft_putstr_fd("'\n", STDERR_FILENO);
			free(op);
			free(tokens[i]);
			tokens[i] = NULL;
			free_array(tokens);
			g_signal = 2;
			return (1);
		}
	}
	return (0);
}

char	*skip_quotes(char *rl)
{
	char	quote;

	quote = *rl++;
	while (*rl && *rl != quote)
		rl++;
	if (*rl)
		rl++;
	return (rl);
}

int	count_tokens(char *rl, int count)
{
	while (*rl)
	{
		while (*rl && ft_isspace(*rl))
			rl++;
		if (!*rl)
			break ;
		count++;
		if (*rl == '\'' || *rl == '"')
			rl = skip_quotes(rl);
		else if (is_operator_start(*rl))
		{
			if (*rl == '<' && *(rl + 1) == '<')
				rl++;
			else if (*rl == '>' && *(rl + 1) == '>')
				rl++;
			rl++;
			continue ;
		}
		while (*rl && !ft_isspace(*rl) && !is_operator_start(*rl))
			rl++;
	}
	return (count);
}
