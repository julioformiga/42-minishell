/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:35:35 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/16 18:47:04 by scarlucc         ###   ########.fr       */
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

typedef enum e_operator {
	OP_NONE,
	OP_PIPE,
	OP_REDIR_IN,
	OP_REDIR_OUT,
	OP_REDIR_APPEND,
	OP_HEREDOC
}	t_operator;

typedef struct s_redirect {
	t_operator			op_type;
	char				*file;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_cmdblock
{
	char				*exec;
	char				**args;
	t_operator			op_type;
	t_redirect			*redirects;
	struct s_cmdblock	*next;
}	t_cmdblock;

typedef struct s_cmd
{
	t_cmdblock		*cmd;
	char			*cmd_line;
}	t_cmd;

typedef int	(*t_builtin_fn)(t_cmd *cmd, t_env *env);

typedef struct s_builtin
{
	char			*name;
	t_builtin_fn	fn;
}	t_builtin;

/* =============================== FUNCTIONS =================================*/
void			signal_handler(int signum);
void			setup_signals(void);

int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strndup(const char *s1, size_t n);

char			**env_to_array(t_env *env);
int				ft_array_len(char **array);
void			free_array(char **array);

t_builtin_fn	get_builtin(char *cmd_name);
int				execute_builtin(t_cmd *cmd, t_env *env,
					int input_fd, int output_fd);

t_env			*env_init(char **envp);
char			*env_get(t_env *env, char *key);
int				key_check(char *key, char *value, int plus);
int				env_set(t_env *env, char *key, char *value, int plus);
int				env_unset(t_env **env, char *key);
void			env_free(t_env *env);

char			**cmd_parser_readline(char *rl);
char			*parser_expansion(const char *str, t_env *env);
t_cmdblock		*create_cmdblock(char *cmd_part);
void			free_cmdblock_content(t_cmdblock *block);
void			free_cmd_content(t_cmd *cmd);
char			*cmd_check(t_cmd *cmd, t_env *env);
void			cmd_parser(char *rl, t_cmd *cmd, t_env *env);

int				is_operator_start(char c);
char			*extract_operator(char **rl);
int				add_redirect(t_cmdblock *block, t_operator type, char *file);

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
