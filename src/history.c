/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 02:30:11 by julio.formiga     #+#    #+#             */
/*   Updated: 2025/01/30 02:30:11 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_history_file_path(t_env *env)
{
	char	*home;
	char	*history_path;

	home = env_get(env, "HOME");
	if (!home)
		return (NULL);
	history_path = ft_strjoin(home, "/.minishell_history");
	free(home);
	return (history_path);
}

void	load_history(t_env *env)
{
	char	*history_path;
	char	*line;
	int		fd;

	history_path = get_history_file_path(env);
	if (!history_path)
		return ;
	fd = open(history_path, O_RDONLY);
	free(history_path);
	if (fd < 0)
		return ;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		line[ft_strlen(line) - 1] = '\0';
		add_history(line);
		free(line);
	}
	close(fd);
}

void	save_history(char *cmd_line, t_env *env)
{
	char	*history_path;
	int		fd;

	if (!cmd_line || !*cmd_line)
		return ;
	history_path = get_history_file_path(env);
	if (!history_path)
		return ;
	fd = open(history_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	free(history_path);
	if (fd < 0)
		return ;
	ft_putendl_fd(cmd_line, fd);
	close(fd);
}
