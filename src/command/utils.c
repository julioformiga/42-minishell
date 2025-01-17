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

char	**env_to_array(t_env *env)
{
	int		count;
	char	**env_array;
	t_env	*current;
	char	*tmp;
	int		i;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = -1;
	while (i++, current)
	{
		tmp = ft_strjoin(current->key, "=");
		env_array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

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

char	*get_operator_str(int op_type)
{
	switch (op_type)
	{
		case OP_PIPE: return ("|");
		case OP_REDIR_IN: return ("<");
		case OP_REDIR_OUT: return (">");
		case OP_REDIR_APPEND: return (">>");
		case OP_HEREDOC: return ("<<");
		default: return ("unknown");
	}
}

void	cmd_debug(t_cmd *cmd)
{
	t_cmdblock	*block;
	t_redirect	*redir;
	int			count_redir;
	int			i;
	int			n;

	block = cmd->cmd;
	printf("\n+-------------------------------------------------------------+\n");
	printf("| Full command: %s\n", cmd->cmd_line);
	i = 0;
	count_redir = 0;
	while (i++, block)
	{
		n = -1;
		printf("|\tCommand #%d: %s\n", i, block->exec);
		if (block->args)
			while (block->args[++n])
				printf("|\t\tArg #%d: %s\n", n + 1, block->args[n]);
		if (block->redirects)
		{
			redir = block->redirects;
			while (redir)
			{
				count_redir++;
				printf("|\t\tRedirect: %s\n", get_operator_str(redir->op_type));
				printf("|\t\t\tFile: %s\n", redir->file);
				redir = redir->next;
			}
		}
		if (block->op_type)
			printf("|\t\tOperator: %s\n", get_operator_str(block->op_type));
		block = block->next;
	}
	printf("+-------------------------------------------------------------+\n");
	printf("| Redirects: %d\n", count_redir);
	printf("+-------------------------------------------------------------+\n");
}
