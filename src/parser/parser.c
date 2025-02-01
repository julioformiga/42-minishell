/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:23:25 by scarlucc          #+#    #+#             */
/*   Updated: 2025/02/01 11:24:52 by scarlucc         ###   ########.fr       */
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

char	*handle_token(char **rl, t_env *env, int *val, int *i)
{
	if (**rl == '\'' || **rl == '"')
		return (extract_quoted_token(rl, env));
	if (is_operator_start(**rl))//forse mettere get_operator_type
	{
		//if ()
			val[*i] = 1;
		return (extract_operator(rl));//in extract_operator togli casistica ||
	}
	return (extract_word(rl, env));
}

int	handle_operator(char *token, char **tokens, int *i)
{
	char	*tmp;

	tmp = ft_strjoin(tokens[*i], token);
	free(tokens[*i]);//forse ridondante
	tokens[*i] = ft_strdup(tmp);
	if (check_previuos_op(token, tokens, *i))
		return (free(tmp), 1);
	free(tmp);
	free(token);
	return (0);
}

void	update_tokens(char **tokens, int *i, char *token, char *rl)
{
	char	*tmp;

	tmp = ft_strjoin(tokens[*i], token);
	free(tokens[*i]);
	tokens[*i] = ft_strdup(tmp);
	if (*rl && (ft_isspace(*rl) || is_operator_start(*rl)))//e se inizia con spazio?
		tokens[++(*i)] = ft_strdup("");
	free(tmp);
	free(token);
}

void	close_matrix_tokens(char **tokens, int *i, int max_args)
{
	if (*i == max_args)
	{
		free(tokens[*i]);
		tokens[*i] = NULL;
	}
	else
		tokens[++(*i)] = NULL;
}

char	**cmd_parser_rl2(char *rl, t_env *env, int *val, int tok_count)
{
	char	**tokens;
	char	*token;
	int		i;

	tokens = init_matrix_tokens(tok_count);
	if (!tokens)
		return (NULL);
	i = 0;
	while (*rl)
	{
		while (*rl && ft_isspace(*rl))
			rl++;
		if (!*rl)
			break ;//fin qui, tutto ok
		token = handle_token(&rl, env, val, &i);
		if (is_operator_start(*token) && handle_operator(token, tokens, &i))
			return (NULL);
		if (is_operator_start(*tokens[i]))//che succede se echo ">" ciao
			tokens[++i] = ft_strdup("");
		else
			update_tokens(tokens, &i, token, rl);
	}
	close_matrix_tokens(tokens, &i, tok_count);
	return (tokens);
}
