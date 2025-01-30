/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 18:25:44 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/30 19:45:39 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expansion(char **rl, t_env *env, char *token, int dq)
{
	char	*expanded;
	char	*tmp;

	if (ft_isspace((*rl)[1]) || (*rl)[1] == '\0' || (dq && (*rl)[1] == '"'))
		expanded = ft_chartostr(*((*rl)++));
	else
		expanded = expand_var(rl, env);
	if (!expanded)
		return (free(token), NULL);
	tmp = ft_strjoin(token, expanded);
	free(token);
	token = tmp;
	if (expanded && ft_strcmp(expanded, "") != 0)
		free(expanded);
	return (token);
}

char	*no_expansion(char **rl, char *token)
{
	char	*tmp;
	char	*str;

	str = ft_chartostr(**rl);
	tmp = ft_strjoin(token, str);
	free(token);
	token = tmp;
	free(str);
	(*rl)++;
	return (token);
}

char	*expand_var(char **rl, t_env *env)
{
	char	*var_name;
	char	*value;
	char	*sig;

	sig = NULL;
	(*rl)++;
	if (**rl == '?')
	{
		(*rl)++;
		sig = ft_itoa(g_signal);
		return (sig);
	}
	var_name = get_var_name(*rl);
	if (!var_name)
		return (NULL);
	value = env_get(env, var_name);
	*rl += ft_strlen(var_name);
	free(var_name);
	return (value);
}

char	*get_var_name(const char *str)
{
	int		i;
	char	*var_name;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	var_name = ft_substr(str, 0, i);
	return (var_name);
}
