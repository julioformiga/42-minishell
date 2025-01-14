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

static char **cmd_parser_readline(char *rl)
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

void	cmd_parser(char *rl, t_cmd *cmd, t_env *env)
{
	t_cmdblock	*block;
	char		**cmd_parts;
	int			i;

	(void)env;
	cmd_parts = cmd_parser_readline(rl);
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
		printf("\033[1;33m");
		cmd_parts[i] = parser_expansion(cmd_parts[i], env);
		printf("Block %d: %s\n", i, cmd_parts[i]);
		printf("\033[0m");
		if (i == 0)
		{
			cmd->cmd = create_cmdblock(cmd_parts[i]);
			if (!cmd->cmd)
			{
				free_array(cmd_parts);
				free(cmd->cmd_line);
				return ;
			}
			if (cmd_parts[i + 1])
				cmd->cmd->args[0] = ft_strdup(cmd_parts[i + 1]);
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
