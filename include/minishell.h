/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:35:35 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/12 18:35:58 by scarlucc         ###   ########.fr       */
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
	char				*op;
	char				*exec;
	char				**args;
	char				**redir; // 0-"> $USER"  1">> asd"
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
void			signal_handler(int signum);
void			setup_signals(void);

char			*ft_strndup(const char *s1, size_t n);

char			**env_to_array(t_env *env);
int				ft_array_len(char **array);
void			free_array(char **array);

t_builtin_fn	get_builtin(char *cmd_name);
int				execute_builtin(t_cmd *cmd, t_env *env,
					int input_fd, int output_fd);

t_env			*env_init(char **envp);
char			*env_get(t_env *env, char *key);
int				env_set(t_env *env, char *key, char *value);
int				env_unset(t_env **env, char *key);
void			env_free(t_env *env);

char			**cmd_parser_readline(char *rl);
char			*parser_expansion(const char *str, t_env *env);
t_cmdblock		*create_cmdblock(char *cmd_part);
void			free_cmdblock_content(t_cmdblock *block);
void			free_cmd_content(t_cmd *cmd);
char			*cmd_check(t_cmd *cmd, t_env *env);
void			cmd_parser(char *rl, t_cmd *cmd, t_env *env);
int				cmd_count_args(char *cmd);
char			**cmd_get_args(char *cmd);
int				cmd_exec(t_cmd *cmd, t_env *env);
int				cmd_setup(t_cmd *cmd, t_env *env, char ***args,
					char **full_path);
void			cmd_init(char *rl, t_cmd *cmd, t_env *env);
void			cmd_exec_inline(int argc, char **argv, t_env *env, t_cmd *cmd);
void			cmd_debug(t_cmd *cmd);
void			cmd_free(t_cmd *cmd);

int				builtin_echo(t_cmd *cmd, t_env *env);
int				builtin_cd(t_cmd *cmd, t_env *env);
int				builtin_export(t_cmd *cmd, t_env *env);
int				builtin_unset(t_cmd *cmd, t_env *env);
int				builtin_env(t_cmd *cmd, t_env *env);
int				builtin_exit(t_cmd *cmd, t_env *env);
int				builtin_pwd(t_cmd *cmd, t_env *env);

#endif
