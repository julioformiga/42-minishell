/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_to_exec_op.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 13:21:45 by scarlucc          #+#    #+#             */
/*   Updated: 2025/02/01 18:29:34 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_str(char **cmd_parts)
{
	int	str_in_matrix;

	str_in_matrix = 0;
	while (cmd_parts[str_in_matrix])
		str_in_matrix++;
	return (str_in_matrix);
}

int	pars_create_new_block(int	*arg_count, t_cmdblock	**current)
{
	(*current)->op_type = OP_PIPE;
	(*current)->next = create_new_block();
	if (!(*current)->next)
		return (1);
	(*current)->next->prev = *current;
	*current = (*current)->next;
	*arg_count = 0;
	return (0);
}

int	cmd_parser_op(char **cmd_parts, int *i, t_cmdblock	**current,
	int	*arg_count)
{
	t_operator	op_type;
	char		*file;

	op_type = get_operator_type(cmd_parts[*i]);
	if (op_type == OP_PIPE)
	{
		if (pars_create_new_block(arg_count, current))
			return (1);
	}
	else if (*i + 1 < (count_str(cmd_parts) + 1))
	{
		if (cmd_parts[*(i) + 1])
			file = ft_strdup(cmd_parts[++*(i)]);
		else
			file = ft_strdup("");
		if (!add_redirect(*current, op_type, file))
		{
			free(file);
			return (1);
		}
		free(file);
	}
	return (0);
}
