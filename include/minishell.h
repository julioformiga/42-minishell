/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:35:35 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/08/30 14:35:35 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <string.h>
# include <termios.h>
# include <curses.h>
# include <dirent.h>

extern int	g_signal;

/* ================================= ENV =====================================*/
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* ================================= CMD =====================================*/
typedef struct s_cmdblock
{
	char				*cmd;
	char				**args;
	char				*sep;
	struct s_cmdblock	*next;
}	t_cmdblock;

typedef struct s_cmd
{
	char		*cmd_line;
	t_cmdblock	*cmd;
}	t_cmd;

/* =============================== FUNCTIONS =================================*/
char	*ft_strndup(const char *s1, size_t n);

void	free_array(char **array);

t_env	*env_init(char **envp);
char	*env_get(t_env *env, char *key);
int		env_set(t_env *env, char *key, char *value);
void	env_print(t_env *env);
void	env_free(t_env *env);

char	*cmd_check(char *path, char *cmd);
int		cmd_exec(char *command, t_env *env);
int		cmd_setup(char *command, t_env *env, char ***args,
			char **full_path);
void	cmd_init(t_cmd *command, char *readline);
void	cmd_exec_inline(int argc, char **argv, t_env **env, t_cmd *cmd);
void	cmd_print(t_cmd *command);
void	cmd_free(t_cmd *cmd);

int		builtin_cd(t_env *env, char *str);
int		builtin_exit(char *str);

#endif
