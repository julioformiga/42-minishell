/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 00:14:41 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/09/21 00:14:41 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(char *str)
{
	if (!str)
		return (1);
	if (ft_strncmp(str, "exit", 4) == 0 && ft_strlen(str) == 4)
		return (0);
	if (ft_strncmp(str, "e", 1) == 0 && ft_strlen(str) == 1)
		return (0);
	return (1);
}
