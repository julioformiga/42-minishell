/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parser_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:48:40 by scarlucc          #+#    #+#             */
/*   Updated: 2025/02/01 21:12:31 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_first_arg(char **cmd_parts, int *i, t_cmdblock	**current)
{
	(*current)->args = malloc(sizeof(char *) * 2);
	if (!(*current)->args)
		return (1);
	(*current)->args[0] = ft_strdup(cmd_parts[*i]);
	if (!(*current)->args[0])
	{
		free((*current)->args);
		(*current)->args = NULL;
		return (1);
	}
	(*current)->args[1] = NULL;
	return (0);
}

int	set_args(char **cmd_parts, int *i, int	*arg_count, t_cmdblock	**current)
{
	int		j;
	char	**temp;

	if (*arg_count == 0)
	{
		if (set_first_arg(cmd_parts, i, current))
			return (1);
	}
	else
	{
		temp = malloc(sizeof(char *) * (*arg_count + 2));
		if (!temp)
			return (1);
		j = -1;
		while (++j < *arg_count)
			temp[j] = (*current)->args[j];
		temp[*arg_count] = ft_strdup(cmd_parts[*i]);
		temp[*arg_count + 1] = NULL;
		free((*current)->args);
		(*current)->args = temp;
	}
	(*arg_count)++;
	return (0);
}
