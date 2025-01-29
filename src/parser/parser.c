/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/01/27 21:04:04 by scarlucc         ###   ########.fr       */
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
		if (*rl == '\'' || *rl == '"')
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
			count++;
		}
		/* else */
			while (*rl && !ft_isspace(*rl) && !is_operator_start(*rl))
				rl++;
	}
	return (count);
}

static char	*add_double_quotes(char *str)
{
	char	*quoted_str;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	quoted_str = malloc(sizeof(char) * (len + 3));
	if (!quoted_str)
		return (NULL);
	quoted_str[0] = '"';
	ft_strlcpy(quoted_str + 1, str, len + 1);
	quoted_str[len + 1] = '"';
	quoted_str[len + 2] = '\0';
	return (quoted_str);
}

static char	*extract_quoted_token(char **rl)
{
	char	quote;
	char	*quoted_token;
	char	*start;
	char	*token;
	int		len;

	quote = **rl;
	start = *rl;
	(*rl)++;
	while (**rl && **rl != quote)
		(*rl)++;
	if (**rl)
	{
		(*rl)++;
		len = *rl - start - 2;
	}
	else
		len = *rl - start - 1;
	token = ft_substr(start, 1, len);
	if (!token)
		return (NULL);
	quoted_token = add_double_quotes(token);
	free(token);
	return (quoted_token);
}

static char	*extract_word(char **rl)
{
	char	*start;
	char	*token;
	int		len;
	/* char	*clean;
	int		i;
	int		j; */

	start = *rl;
	while (**rl && !ft_isspace(**rl) && !is_operator_start(**rl)
		&& !(**rl == '\'' || **rl == '"'))
		(*rl)++;
	len = *rl - start;
	token = ft_substr(start, 0, len);
	if (!token)
		return (NULL);
	/* clean = malloc(sizeof(char) * (len + 1)); //vecchoi codice per estrarre stringhe
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
	free(token); */
	return (token);
}

//echo $' ciao   3spazi   $USER  $HOME  a'b c  d
//stampa uno spazio dopo a' che non deve
/* static char	*do_not_expand(char **rl)
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
	char	*tmp;
	int		token_count;
	int		i;

	token_count = count_tokens(rl);
	tokens = malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	tokens[i] = ft_strdup("");
	while (*rl)
	{
		while (*rl && ft_isspace(*rl))
			rl++;
		if (!*rl)
			break ;
		if (*rl == '$' && (*(rl + 1) == '"' || *(rl + 1) == '\''))//togliere dopo
			rl++;
		if (*rl == '\'' || *rl == '"')
			token = extract_quoted_token(&rl);
		else if (is_operator_start(*rl))
		{
			token = extract_operator(&rl);
			tmp = ft_strjoin(tokens[i], token);
			if (tokens[i])
				free(tokens[i]);
			tokens[i] = tmp;
			if (*rl && !ft_isspace(*rl))
				tokens[++i] = ft_strdup("");
			free(token);
			token = ft_strdup("");
		}
		else
			token = extract_word(&rl);
		tmp = ft_strjoin(tokens[i], token);
		if (tokens[i])
				free(tokens[i]);
		tokens[i] = tmp;
		if (!tokens[i])
		{
			free_array(tokens);
			free(token);
			return (NULL);
		}
		if (*rl && (ft_isspace(*rl) || is_operator_start(*rl)))
			tokens[++i] = ft_strdup("");
		free(token);
	}
	if (ft_isspace(*(rl - 1)))
	{
		free(tokens[i]);
		tokens[i] = NULL;
	}
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
		printf("cmd_parts[%d]: %s\n", i, cmd_parts[i]);
		// printf("cmd_parts[%d]: %s\n", i, cmd_parts[i]);
		//if (is_operator_start(cmd_parts[i][0]))
		if (get_operator_type(cmd_parts[i]) != OP_NONE)//controllo redirect problematico
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
			else if (i + 1 < count_tokens(rl))
			{
				if (cmd_parts[i + 1])
					file = parser_expansion(cmd_parts[++i], env);
				else
					file = ft_strdup("");
				if (!add_redirect(current, op_type, file))
				{
					free(file);
					break ;
				}
				free(file);
			}
		}
		else if (!current->exec)
		{
			if (ft_strncmp(cmd_parts[i], "$", 2) == 0)
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
			current->exec = ft_strdup(cmd_parts[i]);
			if (!current->exec)
				break ;
		}
		else if(cmd_parts[i] && ft_strlen(cmd_parts[i]) > 0)
		{
			if (ft_strncmp(cmd_parts[i], "$", 2) == 0)
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
