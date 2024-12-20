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
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <string.h>
# include <termios.h>
# include <curses.h>
# include <dirent.h>

# define DEBUG 1

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
	char				*exec;
	char				**args;
	char				*separator;
	struct s_cmdblock	*next;
}	t_cmdblock;

typedef struct s_cmd
{
	t_cmdblock		*cmd;
	char			*cmd_line;
}	t_cmd;

typedef int	(*t_builtin_fn)(t_cmd *cmd, t_env *env);

typedef struct sbuiltin
{
	char			*name;
	t_builtin_fn	fn;
}	t_builtin;
/* =============================== FUNCTIONS =================================*/
void	signal_handler(int signum);
void	setup_signals(void);

char	*ft_strndup(const char *s1, size_t n);

int		ft_array_len(char **array);
void	free_array(char **array);

t_env	*env_init(char **envp);
char	*env_get(t_env *env, char *key);
int		env_set(t_env *env, char *key, char *value);
int		env_unset(t_env **env, char *key);
void	env_print(t_env *env);
void	env_free(t_env *env);

char	*cmd_check(t_cmd *cmd, t_env *env);
void	cmd_parser(char *readline, t_cmd *command);
int		cmd_count_args(char *cmd);
char	**cmd_get_args(char *cmd);
int		cmd_exec(t_cmd *cmd, t_env *env);
int		cmd_setup(t_cmd *cmd, t_env *env, char ***args,
			char **full_path);
void	cmd_init(char *readline, t_cmd *command);
void	cmd_exec_inline(int argc, char **argv, t_env *env, t_cmd *cmd);
void	cmd_print(t_cmd *cmd);
void	cmd_free(t_cmd *cmd);

int		builtin_echo(t_cmd *cmd, t_env *env);
int		builtin_cd(t_cmd *cmd, t_env *env);
int		builtin_export(t_cmd *cmd, t_env *env);
int		builtin_unset(t_cmd *cmd, t_env *env);
int		builtin_env(t_cmd *cmd, t_env *env);
int		builtin_exit(t_cmd *cmd, t_env *env);

#endif
