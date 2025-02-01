
#include "minishell.h"

char	**old_cmd_parser_rl(char *rl, t_env *env, int *val, int tok_count)
{
	char	**tokens;
	char	*token;
	char	*tmp;
	int		i;

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
			if (check_previuos_op(token, tokens, i))
				return (free(tmp), NULL);
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

/* static t_cmdblock	*create_new_block(void)
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
} */

void	old_cmd_parser(char *rl, t_cmd *cmd, t_env *env)
{
	t_cmdblock	*current;
	t_operator	op_type;
	char		*file;
	char		**temp;
	char		**cmd_parts;
	int			arg_count;
	int			i;
	int			j;
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
		return ;
	}
	arg_count = 0;
	i = 0;
	while (cmd_parts[i])
	{
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
			//else if (i + 1 < (count_tokens(rl) + 1))
			else if ((i + 1) < (n_tokens + 1))
			{
				if (cmd_parts[i + 1])
					//file = parser_expansion(cmd_parts[++i], env);//change after moving expansion to cmd_parser_readline
					file = ft_strdup(cmd_parts[++i]);//in caso di redirect vuoto, file e' ""
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
