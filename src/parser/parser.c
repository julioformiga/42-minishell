/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/01/30 14:24:18 by scarlucc         ###   ########.fr       */
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
			//se non ho gia' contato questo carattere
			//count++;
			continue;
		}
		while (*rl && !ft_isspace(*rl) && !is_operator_start(*rl))
			rl++;
	}
	return (count);
}

static char	*expand_var(char **rl, t_env *env)
{
	char	*var_name;
	char	*value;
	char	*sig;

	sig = NULL;
	(*rl)++;
	if (**rl == '?')
	{
		(*rl)++;
		sig = ft_itoa(g_signal);
		return (sig);
	}
	var_name = get_var_name(*rl);
	if (!var_name)
		return (NULL);
	value = env_get(env, var_name);
	*rl += ft_strlen(var_name);
	free(var_name);
	return (value);
}

static char *expansion(char **rl, t_env *env, char *token, int dq)
{
	char	*expanded;
	char	*tmp;
	
	if (ft_isspace((*rl)[1]) || (*rl)[1] == '\0' || (dq && (*rl)[1] == '"'))
		expanded = ft_chartostr(*((*rl)++));
	else
		expanded = expand_var(rl, env);
	if (!expanded)
		return (free(token), NULL);
	tmp = ft_strjoin(token, expanded);
	free(token);
	token = tmp;
	if (expanded && ft_strcmp(expanded, "") != 0)
		free(expanded);
	return (token);
}

static char *no_expansion(char **rl, char *token)
{
	char	*tmp;
	char	*str;

	str = ft_chartostr(**rl);
	tmp = ft_strjoin(token, str);
	free(token);
	token = tmp;
	free(str);
	(*rl)++;
	return (token);
}

static char	*extract_quoted_token(char **rl, t_env *env)
{
	char	quote;
	char	*token;

	token = ft_strdup("");
	if (!token)
		return (NULL);
	quote = **rl;
	(*rl)++;
	while (**rl && **rl != quote)
	{
		if (**rl == '$' && quote == '"')
			token = expansion(rl, env, token, 1);
		else
			token = no_expansion(rl, token);
	}
	if (**rl == quote)
		(*rl)++;
	return (token);
}

static char	*extract_word(char **rl, t_env *env)
{
	char	*token;

	token = ft_strdup("");
	if (!token)
		return (NULL);
	while (**rl && !ft_isspace(**rl) && !is_operator_start(**rl)
		&& !(**rl == '\'' || **rl == '"'))
	{
		if (**rl == '$')
			token = expansion(rl, env, token, 0);
		else
			token = no_expansion(rl, token);
	}
	return (token);
}

char	**cmd_parser_rl(char *rl, t_env *env, int *val, int tok_count)
{
	char	**tokens;
	char	*token;
	char	*tmp;
	//int		token_count;
	int		i;

	//token_count = count_tokens(rl);
	tokens = malloc(sizeof(char *) * (tok_count + 1));
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
		if (*rl == '\'' || *rl == '"')
			token = extract_quoted_token(&rl, env);
		else if (is_operator_start(*rl))
		{
			token = extract_operator(&rl);
			tmp = ft_strjoin(tokens[i], token);
			if (tokens[i])
				free(tokens[i]);
			tokens[i] = tmp;
			val[i] = 1;
			if (*rl && !ft_isspace(*rl))
				tokens[++i] = ft_strdup("");
			free(token);
			token = ft_strdup("");
		}
		else
			token = extract_word(&rl, env);
		tmp = ft_strjoin(tokens[i], token);
		if (tokens[i])
				free(tokens[i]);
		tokens[i] = tmp;
		if (!tokens[i])//se tokens[i] == "" che succede?
			return (free_array(tokens), free(token), NULL);
		if (*rl && (ft_isspace(*rl) || is_operator_start(*rl)))
			tokens[++i] = ft_strdup("");
		free(token);
	}
	if (ft_isspace(*(rl - 1)))//in caso il comando finisca con spazi vuoti
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
	int			*values;
	int			n_tokens;

	n_tokens = count_tokens(rl);
	values = ft_calloc(n_tokens, sizeof(int));
	cmd_parts = cmd_parser_rl(rl, env, values, n_tokens);
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
		expanded = ft_strdup(cmd_parts[i]);
		if (!expanded)
		{
			free_array(cmd_parts);
			return ;
		}
		free(cmd_parts[i]);
		cmd_parts[i] = ft_strdup(expanded);//il problema e' qui. Rimuovere dopo spostamento expansion
		free(expanded);//Rimuovere dopo spostamento expansion
		//if (is_operator_start(cmd_parts[i][0]))//vecchia condizione per controllo redirect problematico
		if ((values[i] == 1) && get_operator_type(cmd_parts[i]) != OP_NONE)//controllo redirect problematico
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
					//file = parser_expansion(cmd_parts[++i], env);//change after moving expansion to cmd_parser_readline
					file = ft_strdup(cmd_parts[++i]);//nuova versione, dopo aver spostato espansione in cmd_parser_readline
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
	free(values);
	free_array(cmd_parts);
}
