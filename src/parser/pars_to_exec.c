/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_to_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:33:42 by scarlucc          #+#    #+#             */
/*   Updated: 2025/02/02 13:07:07 by scarlucc         ###   ########.fr       */
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
	block->fd_in = -1;
	block->fd_out = -1;
	block->args = NULL;
	block->op_type = OP_NONE;
	block->redirects = NULL;
	block->next = NULL;
	block->prev = NULL;
	return (block);
}

static	t_cmds_values	*init_cmd_values(char *rl, t_env *env, int	*n_tokens)
{
	t_cmds_values	*cmd_val;

	*n_tokens = count_tokens(rl, 0);
	cmd_val = ft_calloc(1, sizeof(t_cmds_values));
	if (!cmd_val)
		return (NULL);
	cmd_val->values = ft_calloc(*n_tokens, sizeof(int));
	if (!cmd_val->values)
	{
		free(cmd_val);
		return (NULL);
	}
	cmd_val->cmd_parts = cmd_parser_rl(rl, env, cmd_val->values, *n_tokens);
	if (!cmd_val->cmd_parts)
	{
		free(cmd_val->values);
		free(cmd_val);
		return (NULL);
	}
	return (cmd_val);
}

t_cmdblock	*process_tokens(t_cmds_values *cmd_val, t_cmdblock	*current, int i)
{
	int				arg_count;

	arg_count = 0;
	while (cmd_val->cmd_parts[i])
	{
		if ((cmd_val->values[i] == 1)
			&& get_operator_type(cmd_val->cmd_parts[i]) != OP_NONE)
		{
			if (cmd_parser_op(cmd_val->cmd_parts, &i, &current, &arg_count))
				break ;
		}
		else if (!current->exec)
		{
			current->exec = ft_strdup(cmd_val->cmd_parts[i]);
			if (!current->exec)
				break ;
		}
		else if (cmd_val->cmd_parts[i] && ft_strlen(cmd_val->cmd_parts[i]) > 0)
		{
			if (set_args(cmd_val->cmd_parts, &i, &arg_count, &current))
				break ;
		}
		i++;
	}
	return (get_first_block(current));
}

void	cmd_parser(char *rl, t_cmd *cmd, t_env *env)
{
	int				i;
	t_cmdblock		*current;
	int				n_tokens;
	t_cmds_values	*cmd_val;

	cmd_val = init_cmd_values(rl, env, &n_tokens);
	if (!cmd_val)
		return ;
	current = create_new_block();
	if (!current)
	{
		free_array(cmd_val->cmd_parts);
		free(cmd_val->values);
		free(cmd_val);
		return ;
	}
	i = 0;
	cmd->cmd = process_tokens(cmd_val, current, i);
	free(cmd_val->values);
	free_array(cmd_val->cmd_parts);
	free(cmd_val);
}
