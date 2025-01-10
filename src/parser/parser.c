/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:17:17 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/01/09 11:17:17 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cmd_parser_readline(char *rl)
{
	int		i = -1;

	printf(">>>>> %s\n", rl);
	while(rl[++i])
	{
		printf("\033[1;34m");
		if (rl[i] == '"' && rl[i - 1] != '\\' & i != 0)
			continue;
		else if (rl[i] == '"' && rl[i - 1] == ' ')
		{
			while (++i, rl[i] != '"' && rl[i - 1] != '\\')
				printf("%c", rl[i]);
		}
		else
			printf("%c", rl[i]);
		printf("\033[0m");
	}
	printf("\n");
}

void	cmd_parser(char *rl, t_cmd *cmd, t_env *env)
{
	t_cmdblock	*block;
	char		**cmd_parts;
	int			i;

	(void)env;
	cmd_parser_readline(rl);
	cmd_parts = ft_split(rl, '|');
	if (!cmd_parts)
	{
		free(cmd->cmd_line);
		cmd->cmd_line = NULL;
		return ;
	}
	i = -1;
	cmd->cmd = NULL;
	while (cmd_parts[++i])
	{
		if (i == 0)
		{
			cmd->cmd = create_cmdblock(cmd_parts[i]);
			if (!cmd->cmd)
			{
				free_array(cmd_parts);
				free(cmd->cmd_line);
				return ;
			}
			block = cmd->cmd;
		}
		else
		{
			block->next = create_cmdblock(cmd_parts[i]);
			if (!block->next)
			{
				free_array(cmd_parts);
				free_cmd_content(cmd);
				return ;
			}
			block = block->next;
		}
	}
	free_array(cmd_parts);
}
