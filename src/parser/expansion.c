/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 18:25:44 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/01/13 18:25:44 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_chartostr(char c)
{
	char	*str;

	str = malloc(2);
	if (!str)
		return (NULL);
	str[0] = c;
	str[1] = '\0';
	return (str);
}

static char *get_var_name(const char *str)
{
	int		i;
	char	*var_name;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	var_name = ft_substr(str, 0, i);
	return (var_name);
}

static char *expand_variable(const char *str, int *i, t_env *env)
{
	char	*var_name;
	char	*value;


	(*i)++;
	var_name = get_var_name(str + *i);
	if (!var_name)
		return (NULL);
	value = env_get(env, var_name);
	*i += ft_strlen(var_name);
	free(var_name);
	return (value);
}

char	*parser_expansion(const char *str, t_env *env)
{
	char	*expanded;
	char	*temp;
	int		i;
	int		in_quotes;
	char	quote_type;
	char	*new_expanded;

	expanded = ft_strdup("");
	if (!expanded)
		return (NULL);
	i = 0;
	in_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				quote_type = str[i];
			}
			else if (str[i] == quote_type)
				in_quotes = 0;
			temp = ft_chartostr(str[i++]);
		}
		else if (str[i] == '$' && (in_quotes == 0 || quote_type == '"'))
		{
			temp = expand_variable(str, &i, env);
			if (!temp)
			{
				free(expanded);
				return (NULL);
			}
		}
		else
		{
			temp = ft_chartostr(str[i++]);
		}
		if (!temp)
		{
			free(expanded);
			return (NULL);
		}
		new_expanded = ft_strjoin(expanded, temp);
		free(expanded);
		free(temp);
		expanded = new_expanded;
		if (!expanded)
			return (NULL);
	}
	return (expanded);
}
