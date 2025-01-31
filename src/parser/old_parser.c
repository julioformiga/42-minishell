
#include "minishell.h"

char	**cmd_parser_rl(char *rl, t_env *env, int *val, int tok_count)
{
	char	**tokens;
	char	*token;
	char	*tmp;
	int		i;

	tokens = malloc(sizeof(char *) * (tok_count + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	tokens[i] = ft_strdup("");
	while (*rl)
	{
		while (*rl && ft_isspace(*rl))
			rl++;
		if (!*rl)
			break ;
		if (*rl == '\'' || *rl == '"')
			token = extract_quoted_token(&rl, env);
		else if (is_operator_start(*rl))
		{
			token = extract_operator(&rl);
			tmp = ft_strjoin(tokens[i], token);
			if (tokens[i])
				free(tokens[i]);
			tokens[i] = tmp;
			if (check_previuos_op(token, tokens, i))
				return (free(tmp), NULL);
			val[i] = 1;
			if (*rl && !ft_isspace(*rl))
				tokens[++i] = ft_strdup("");
			free(token);
			token = ft_strdup("");
		}
		else
			token = extract_word(&rl, env);
		tmp = ft_strjoin(tokens[i], token);
		if (tokens[i])
				free(tokens[i]);
		tokens[i] = tmp;
		if (!tokens[i])//se tokens[i] == "" che succede?
			return (free_array(tokens), free(token), NULL);
		if (*rl && (ft_isspace(*rl) || is_operator_start(*rl)))
			tokens[++i] = ft_strdup("");
		free(token);
	}
	if (ft_isspace(*(rl - 1)))//in caso il comando finisca con spazi vuoti
	{
		free(tokens[i]);
		tokens[i] = NULL;
	}
	else
		tokens[++i] = NULL;
	return (tokens);
}
