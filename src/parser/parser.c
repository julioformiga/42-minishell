/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/01/25 19:17:49 by scarlucc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_tokens(char *rl)
{
	int		count;
	char	quote;

	count = 0;
	while (*rl)
	{
		while (*rl && ft_isspace(*rl))
			rl++;
		if (!*rl)
			break ;
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
		/* else */
			while (*rl && !ft_isspace(*rl) && !is_operator_start(*rl))
				rl++;
	}
	return (count);
}

static char	*extract_quoted_token(char **rl)
{
	char	quote;
	char	*start;
	char	*token;
	int		len;

	quote = **rl;
	start = *rl;
	(*rl)++;
	while (**rl && **rl != quote)//espansione in questo ciclo
		(*rl)++;
	if (**rl)
	{
		(*rl)++;
		len = *rl - start - 2;
	}
	else
		len = *rl - start - 1;
	token = ft_substr(start, 1, len);
	return (token);
}

static char	*extract_word(char **rl)
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
	clean = malloc(sizeof(char) * (len + 1));
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

//echo $' ciao   3spazi   $USER  $HOME  a'b c  d
//stampa uno spazio dopo a' che non deve
/* static char	*extract_single_quote(char **rl)
{
	char	*str_to_check;
	char	*start;
	int 	len;

	start = *rl;
	(*rl)++;
	while (**rl && **rl != '\'')
		(*rl)++;
	(*rl)++;
	len = *rl - start;
	str_to_check = ft_substr(start, 0, len);
	return (str_to_check);
} */

char	**cmd_parser_readline(char *rl)
{
	char	**tokens;
	char	*token;
	int		token_count;
	int		i;

	token_count = count_tokens(rl);
	tokens = malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	tokens[i] = "";
	while (*rl)
	{
		while (*rl && ft_isspace(*rl))
			rl++;
		if (!*rl)
			break ;
		if (*rl == '$' && (*(rl + 1) == '"' || *(rl + 1) == '\''))//forse togliere o mettere dentro extract_word()
			rl++;
		if (*rl == '\'' || *rl == '"'/*  && (ft_isspace(*(rl - 1))) */)
			token = extract_quoted_token(&rl);
		else if (is_operator_start(*rl))//capire bene
			token = extract_operator(&rl);
		else
			token = extract_word(&rl);
		tokens[i] = ft_strjoin(tokens[i], token);
		if (!tokens[i])
		{
			free_array(tokens);
			free(token);
			return (NULL);
		}
		if (*rl && ft_isspace(*rl))
			tokens[++i] = "";
		free(token);
	}
	if (ft_isspace(*(rl - 1)))
		tokens[i] = NULL;
	else
		tokens[++i] = NULL;
	return (tokens);
}

static t_cmdblock	*create_new_block(void)
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

static t_operator	get_operator_type(const char *op)
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

void	cmd_parser(char *rl, t_cmd *cmd, t_env *env)
{
	t_cmdblock	*current;
	t_operator	op_type;
	char		*file;
	char		*expanded;
	char		**temp;
	char		**cmd_parts;
	int			arg_count;
	int			i;
	int			j;

	cmd_parts = cmd_parser_readline(rl);
	if (!cmd_parts)
	{
		free(cmd->cmd_line);
		cmd->cmd_line = NULL;
		return ;
	}
	current = create_new_block();
	if (!current)
	{
		free_array(cmd_parts);
		return ;
	}
	arg_count = 0;
	i = 0;
	while (cmd_parts[i])
	{
		if (ft_strncmp(cmd_parts[i], "$", 2) == 0)//puo' essere generalizzato in espansione
			expanded = ft_strdup("$");
		else
			expanded = parser_expansion(cmd_parts[i], env);
		if (!expanded)
		{
			free_array(cmd_parts);
			return ;
		}
		free(cmd_parts[i]);
		cmd_parts[i] = expanded;
		if (is_operator_start(cmd_parts[i][0]))//che succede se echo ">prova.txt" giacomo
		{
			op_type = get_operator_type(cmd_parts[i]);
			if (op_type == OP_PIPE)
			{
				current->op_type = OP_PIPE;
				current->next = create_new_block();
				if (!current->next)
					break ;
				current->next->prev = current;
				current = current->next;
				arg_count = 0;
			}
			else if (i + 1 < count_tokens(rl))//chiedere a Julio perche' questa cosa
			{
				i++;
				file = parser_expansion(cmd_parts[i], env);
				if (!file || !add_redirect(current, op_type, file))
				{
					free(file);
					break ;
				}
				free(file);
			}
		}
		else if (!current->exec)
		{
			current->exec = ft_strdup(cmd_parts[i]);
			if (!current->exec)
				break ;
		}
		else if(cmd_parts[i] && ft_strlen(cmd_parts[i]) > 0)
		{
			if (arg_count == 0)
			{
				current->args = malloc(sizeof(char *) * 2);
				if (!current->args)
					break ;
				current->args[0] = ft_strdup(cmd_parts[i]);
				if (!current->args[0])
				{
					free(current->args);
					current->args = NULL;
					break ;
				}
				current->args[1] = NULL;
			}
			else
			{
				temp = malloc(sizeof(char *) * (arg_count + 2));
				if (!temp)
					break ;
				j = -1;
				while (j++, j < arg_count)
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
	cmd->cmd = get_first_block(current);
	free_array(cmd_parts);
}
