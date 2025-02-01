/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_to_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:33:42 by scarlucc          #+#    #+#             */
/*   Updated: 2025/02/01 21:22:57 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmdblock	*create_new_block(void)
{
	t_cmdblock	*block;

	block = malloc(sizeof(t_cmdblock) * 1);
	if (!block)
		return (NULL);
	block->exec = NULL;
	block->args = NULL;
	block->op_type = OP_NONE;
	block->redirects = NULL;
	block->next = NULL;
	block->prev = NULL;
	return (block);
}

void	cmd_parser(char *rl, t_cmd *cmd, t_env *env)
{
	int			i;
	char		**cmd_parts;
	int			arg_count;
	t_cmdblock	*current;
	int			*values;
	int			n_tokens;

	n_tokens = count_tokens(rl, 0);
	values = ft_calloc(n_tokens, sizeof(int));
	cmd_parts = cmd_parser_rl(rl, env, values, n_tokens);
	if (!cmd_parts)
	{
		free(values);
		return ;
	}
	current = create_new_block();
	if (!current)
	{
		free_array(cmd_parts);
		free(values);
		return ;
	}
	arg_count = 0;
	i = 0;
	while (cmd_parts[i])
	{
		if ((values[i] == 1) && get_operator_type(cmd_parts[i]) != OP_NONE)
		{
			if (cmd_parser_op(cmd_parts, &i, &current, &arg_count))
				break ;
		}
		else if (!current->exec)
		{
			current->exec = ft_strdup(cmd_parts[i]);
			if (!current->exec)
				break ;
		}
		else if(cmd_parts[i] && ft_strlen(cmd_parts[i]) > 0)
			if (set_args(cmd_parts, &i, &arg_count, &current))
				break ;
		i++;
	}
	cmd->cmd = get_first_block(current);
	free(values);
	free_array(cmd_parts);
}
