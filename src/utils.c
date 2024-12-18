/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:17:32 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/09/16 16:17:32 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
	array = NULL;
}

void	cmd_free(t_cmd *cmd)
{
	t_cmd	*current;
	t_cmd	*next;

	if (!cmd)
		return ;
	current = cmd;
	while (current)
	{
		next = current->next;
		if (current->cmd_line)
			free(current->cmd_line);
		if (current->cmd)
		{
			if (current->cmd->args)
				free_array(current->cmd->args);
			if (current->cmd->exec)
				free(current->cmd->exec);
			if (current->cmd->sep)
				free(current->cmd->sep);
			free(current->cmd);
		}
		free(current);
		current = next;
	}
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*str;
	size_t	i;

	if (!s1 || n == 0)
		return (NULL);
	str = malloc(sizeof(char) * (n + 1));
	if (str == NULL)
		return (NULL);
	i = -1;
	while (++i < n)
		str[i] = s1[i];
	str[i] = '\0';
	return (str);
}
