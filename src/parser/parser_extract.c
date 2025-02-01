/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_extract.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:03:16 by scarlucc          #+#    #+#             */
/*   Updated: 2025/02/01 12:13:18 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_quoted_token(char **rl, t_env *env)
{
	char	quote;
	char	*token;

	token = ft_strdup("");
	if (!token)
		return (NULL);
	quote = **rl;
	(*rl)++;
	while (**rl && **rl != quote)
	{
		if (**rl == '$' && quote == '"')
			token = expansion(rl, env, token, 1);
		else
			token = no_expansion(rl, token);
	}
	if (**rl == quote)
		(*rl)++;
	return (token);
}

char	*extract_word(char **rl, t_env *env)
{
	char	*token;

	token = ft_strdup("");
	if (!token)
		return (NULL);
	while (**rl && !ft_isspace(**rl) && !is_operator_start(**rl)
		&& !(**rl == '\'' || **rl == '"'))
	{
		if (**rl == '$')
			token = expansion(rl, env, token, 0);
		else
			token = no_expansion(rl, token);
	}
	return (token);
}

char	*extract_operator(char **rl)
{
	char	*token;

	/* if (**rl == '|' && *(*rl + 1) == '|')//commentato per far interpretare al parsing || come se fosse | |
	{
		token = ft_strdup("||");
		(*rl)++;
	}
	else if (**rl == '|') */
	if (**rl == '|')
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
