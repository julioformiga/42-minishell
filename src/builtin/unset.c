/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:12:09 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/12/17 15:12:09 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_cmd *cmd, t_env *env)
{
	int	i;

	i = -1;
	while (cmd->cmd->args[++i])
		env_unset(&env, cmd->cmd->args[i]);
	return (1);
}
