/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 17:32:49 by scarlucc          #+#    #+#             */
/*   Updated: 2025/01/12 19:33:38 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_cmd *cmd, t_env *env)
{
	char	cwd[4096];

	(void)cmd;
	(void)env;
	if (getcwd(cwd, 4096) != 0)
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (0);
	}
	else
	{
		ft_putstr_fd("Error:	pwd error\n\0", STDERR_FILENO);
		return (1);
	}
}
