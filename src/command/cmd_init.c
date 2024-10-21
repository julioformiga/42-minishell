/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 08:34:04 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/09/18 08:34:04 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cmd_check(char *path, char *cmd)
{
	char	*full_path;
	char	*full_path_cmd;
	char	**paths;
	int		i;

	paths = ft_split(path, ':');
	i = -1;
	while (paths[++i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path_cmd = ft_strjoin(full_path, cmd);
		free(full_path);
		if (access(full_path_cmd, X_OK) == 0)
		{
			free_array(paths);
			return (full_path_cmd);
		}
		free(full_path_cmd);
	}
	printf("%s: command not found\n", cmd);
	free_array(paths);
	return (NULL);
}

int	cmd_setup(char *command, t_env *env, char ***args, char **full_path)
{
	char	*path;

	path = env_get(env, "PATH");
	*full_path = cmd_check(path, command);
	if (!*full_path)
		return (1);
	*args = ft_split(command, ' ');
	return (0);
}

int	count_args(char *cmd)
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

t_cmdblock	*create_cmdblock(char *cmd_part)
{
	t_cmdblock	*new_block;
	char		**args;
	int			i;

	new_block = malloc(sizeof(t_cmdblock));
	args = ft_split(cmd_part, ' ');
	new_block->cmd = ft_strdup(args[0]);
	new_block->args = malloc(sizeof(char *) * (count_args(cmd_part) + 1));
	for (i = 1; args[i]; i++)
		new_block->args[i - 1] = ft_strdup(args[i]);
	new_block->args[i - 1] = NULL;
	new_block->sep = NULL;
	new_block->next = NULL;
	free_array(args);
	return (new_block);
}

void	cmd_init(t_cmd *command, char *readline)
{
	char *separators[] = {"|", "||", ">", ">>", "<", "<<", "&&", "&", NULL};
	t_cmdblock	*head = NULL;
	t_cmdblock	*current = NULL;
	char		*start = readline;
	char		*end;
	int			i;

	command->cmd_line = ft_strdup(readline);
	while (*start) {
		while (*start && isspace(*start)) start++;
		if (!*start) break;

		end = start;
		while (*end) {
			int found_separator = 0;
			for (i = 0; separators[i]; i++) {
				if (strncmp(end, separators[i], strlen(separators[i])) == 0) {
					found_separator = 1;
					break;
				}
			}
			if (found_separator)
				break;
			end++;
		}
		char *cmd_part = strndup(start, end - start);
		t_cmdblock *new_block = create_cmdblock(cmd_part);
		free(cmd_part);

		if (!head)
			head = new_block;
		else
			current->next = new_block;
		current = new_block;

		if (*end) {
			for (i = 0; separators[i]; i++) {
				if (strncmp(end, separators[i], strlen(separators[i])) == 0) {
					current->sep = ft_strdup(separators[i]);
					end += strlen(separators[i]);
					break;
				}
			}
		}

		start = end;
	}
	command->cmd = head;
}

void	cmd_print(t_cmd *command)
{
	t_cmdblock	*block;

	block = command->cmd;
	printf("Comando completo: %s\n", command->cmd_line);
	while (block)
	{
		printf("\tComando: %s\n", block->cmd);
		for (int i = 0; block->args[i] != NULL; i++)
			printf("\tArg[%d]: %s\n", i, block->args[i]);
		if (block->sep)
			printf("\tSeparador: %s\n", block->sep);
		block = block->next;
	}
}
