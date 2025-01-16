/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 18:00:30 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/01/15 18:00:30 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

static int is_operator_start(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int count_tokens(char *rl)
{
	int		count;
	char	quote;

	count = 0;
	while (*rl)
	{
		while (*rl && ft_isspace(*rl))
			rl++;
		if (!*rl)
			break;
		count++;
		if ((*rl == '\'' || *rl == '"') && (ft_isspace(*(rl - 1))))
		{
			quote = *rl++;
			while (*rl && *rl != quote)
				rl++;
			if (*rl)
				rl++;
		}
		else if (is_operator_start(*rl))
		{
			if (*rl == '<' && *(rl + 1) == '<')
				rl++;
			else if (*rl == '>' && *(rl + 1) == '>')
				rl++;
			rl++;
		}
		else
			while (*rl && !ft_isspace(*rl) && !is_operator_start(*rl))
				rl++;
	}
	return (count);
}

static char *extract_quoted_token(char **rl)
{
	char	quote;
	char	*start;
	char	*token;
	int		len;

	quote = **rl;
	start = *rl;
	(*rl)++;
	while (**rl && **rl != quote)
		(*rl)++;
	if (**rl)
		(*rl)++;
	len = *rl - start;
	token = ft_substr(start, 0, len);
	return (token);
}

static char *extract_operator(char **rl)
{
	char	*token;

	if (**rl == '|')
		token = ft_strdup("|");
	else if (**rl == '<' && *(*rl + 1) == '<')
	{
		token = ft_strdup("<<");
		(*rl)++;
	}
	else if (**rl == '>' && *(*rl + 1) == '>')
	{
		token = ft_strdup(">>");
		(*rl)++;
	}
	else if (**rl == '<')
		token = ft_strdup("<");
	else
		token = ft_strdup(">");
	(*rl)++;
	return (token);
}

static char *extract_word(char **rl)
{
	char	*start;
	char	*token;
	char	*clean;
	int		len;
	int		i;
	int		j;

	start = *rl;
	while (**rl && !ft_isspace(**rl) && !is_operator_start(**rl))
		(*rl)++;
	len = *rl - start;
	token = ft_substr(start, 0, len);
	if (!token)
		return (NULL);

	clean = malloc(len + 1);
	if (!clean)
	{
		free(token);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (token[i])
	{
		if (token[i] != '"' && token[i] != '\'')
			clean[j++] = token[i];
		i++;
	}
	clean[j] = '\0';
	free(token);
	return (clean);
}

char **cmd_parser_readline(char *rl)
{
	char	**tokens;
	int		token_count;
	int		i;

	token_count = count_tokens(rl);
	tokens = malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	while (*rl)
	{
		while (*rl && ft_isspace(*rl))
			rl++;
		if (!*rl)
			break;
		if ((*rl == '\'' || *rl == '"') && (ft_isspace(*(rl - 1))))
			tokens[i] = extract_quoted_token(&rl);
		else if (is_operator_start(*rl))
			tokens[i] = extract_operator(&rl);
		else
			tokens[i] = extract_word(&rl);
		if (!tokens[i])
		{
			free_array(tokens);
			return (NULL);
		}
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}

static t_cmdblock *create_new_block(void)
{
	t_cmdblock *block;

	block = malloc(sizeof(t_cmdblock));
	if (!block)
		return (NULL);
	block->exec = NULL;
	block->args = NULL;
	block->op_type = OP_NONE;
	block->redirects = NULL;
	block->next = NULL;
	return (block);
}

static void	free_cmdblock(t_cmdblock *block)
{
	t_cmdblock	*next;

	while (block)
	{
		next = block->next;
		if (block->exec)
			free(block->exec);
		if (block->args)
			free_array(block->args);
		free(block);
		block = next;
	}
}

static t_operator get_operator_type(const char *op)
{
	if (!op)
		return (OP_NONE);
	if (ft_strcmp(op, "|") == 0)
		return (OP_PIPE);
	if (ft_strcmp(op, "<") == 0)
		return (OP_REDIR_IN);
	if (ft_strcmp(op, ">") == 0)
		return (OP_REDIR_OUT);
	if (ft_strcmp(op, ">>") == 0)
		return (OP_REDIR_APPEND);
	if (ft_strcmp(op, "<<") == 0)
		return (OP_HEREDOC);
	return (OP_NONE);
}

static t_redirect *create_redirect(t_operator type, char *file)
{
	t_redirect *redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->op_type = type;
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

static int add_redirect(t_cmdblock *block, t_operator type, char *file)
{
	t_redirect *new_redir;
	t_redirect *current;

	new_redir = create_redirect(type, file);
	if (!new_redir)
		return (0);

	if (!block->redirects)
		block->redirects = new_redir;
	else
	{
		current = block->redirects;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
	return (1);
}

void	cmd_parser(char *rl, t_cmd *cmd, t_env *env)
{
	t_cmdblock	*current;
	t_cmdblock	*first;
	char		**cmd_parts;
	int			arg_count;
	int			i;

	cmd_parts = cmd_parser_readline(rl);
	if (!cmd_parts)
	{
		free(cmd->cmd_line);
		cmd->cmd_line = NULL;
		return;
	}

	first = create_new_block();
	if (!first)
	{
		free_array(cmd_parts);
		return;
	}

	current = first;
	arg_count = 0;
	i = 0;

	while (cmd_parts[i])
	{
		char *expanded = parser_expansion(cmd_parts[i], env);
		if (!expanded)
		{
			free_cmdblock(first);
			free_array(cmd_parts);
			return;
		}
		free(cmd_parts[i]);
		cmd_parts[i] = expanded;

		if (is_operator_start(cmd_parts[i][0]))
		{
			t_operator op_type = get_operator_type(cmd_parts[i]);

			if (op_type == OP_PIPE)
			{
				current->op_type = OP_PIPE;
				current->next = create_new_block();
				if (!current->next)
					break;
				current = current->next;
				arg_count = 0;
			}
			else if (i + 1 < count_tokens(rl))  // Verifica se há um arquivo após o redirecionador
			{
				i++;  // Avança para o nome do arquivo
				char *file = parser_expansion(cmd_parts[i], env);
				if (!file || !add_redirect(current, op_type, file))
				{
					free(file);
					break;
				}
				free(file);
			}
		}
		else if (!current->exec)
		{
			current->exec = ft_strdup(cmd_parts[i]);
			if (!current->exec)
				break;
		}
		else
		{
			if (arg_count == 0)
			{
				current->args = malloc(sizeof(char *) * 2);
				if (!current->args)
					break;
				current->args[0] = ft_strdup(cmd_parts[i]);
				if (!current->args[0])
				{
					free(current->args);
					current->args = NULL;
					break;
				}
				current->args[1] = NULL;
			}
			else
			{
				char **temp = malloc(sizeof(char *) * (arg_count + 2));
				if (!temp)
					break;
				for (int j = 0; j < arg_count; j++)
					temp[j] = current->args[j];
				temp[arg_count] = ft_strdup(cmd_parts[i]);
				temp[arg_count + 1] = NULL;
				free(current->args);
				current->args = temp;
			}
			arg_count++;
		}
		i++;
	}
	cmd->cmd = first;
	free_array(cmd_parts);
}
