/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:11:10 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/11/18 15:11:10 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_count_args(char *cmd)
{
	int		count;
	char	*token;

	count = 0;
	token = strtok(cmd, " ");
	while (token != NULL)
	{
		count++;
		token = strtok(NULL, " ");
	}
	return (count);
}

char	**cmd_get_args(char *cmd)
{
	char	**args;
	char	*token;
	int		i;
	char	*cmd_copy;

	cmd_copy = ft_strdup(cmd);
	args = malloc(sizeof(char *) * (cmd_count_args(cmd_copy) + 1));
	if (!args)
	{
		free(cmd_copy);
		return (NULL);
	}
	token = strtok(cmd_copy, " ");
	i = 0;
	while (token != NULL)
	{
		args[i] = ft_strdup(token);
		token = strtok(NULL, " ");
		i++;
	}
	args[i] = NULL;
	free(cmd_copy);
	return (args);
}
