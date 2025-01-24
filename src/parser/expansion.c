/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 18:25:44 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/24 19:16:13 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_name(const char *str)
{
	int		i;
	char	*var_name;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	var_name = ft_substr(str, 0, i);
	return (var_name);
}

static char	*expand_variable(const char *str, int *i, t_env *env)
{
	char	*var_name;
	char	*value;
	char	*sig;

	sig = NULL;
	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		sig = ft_itoa(g_signal);
		return (sig);
	}
	var_name = get_var_name(str + *i);
	if (!var_name)
		return (NULL);
	value = env_get(env, var_name);
	*i += ft_strlen(var_name);
	free(var_name);
	return (value);
}

static char	*parser_part_removing_quotes_begin_end(char *str)
{
	char	*new_str;
	int		i;
	int		j;

	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	if (str[i] == '\'' || str[i] == '"')
		i++;
	while (str[i])
	{
		if ((str[i] == '\'' && str[i + 1] == '\0')
			|| (str[i] == '"' && str[i + 1] == '\0'))
		{
			break ;
		}
		new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (new_str);
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
	temp = NULL;
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
		if (temp && temp[0] != '\0')
			free(temp);
		expanded = new_expanded;
		if (!expanded)
			return (NULL);
	}
	temp = parser_part_removing_quotes_begin_end(expanded);
	free(expanded);
	return (temp);
}
