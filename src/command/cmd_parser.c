/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:21:23 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/11/20 14:21:23 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_parser(char *readline, t_cmd *command)
{
	char	*cmd_line;

	cmd_line = ft_strdup(readline);
	command->cmd_line = cmd_line;
	free(cmd_line);
}
