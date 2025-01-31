/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlucc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:35:35 by julio.formi       #+#    #+#             */
/*   Updated: 2025/01/31 14:57:19 by scarlucc         ###   ########.fr       */
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

typedef enum e_operator
{
	OP_NONE,
	OP_PIPE,
	OP_REDIR_IN,
	OP_REDIR_OUT,
	OP_REDIR_APPEND,
	OP_HEREDOC
}	t_operator;

typedef struct s_redirect
{
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
	struct s_cmdblock	*prev;
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

void			load_history(t_env *env);
void			save_history(char *cmd_line, t_env *env);

char			**env_to_array(t_env *env);
int				ft_array_len(char **array);
void			free_array(char **array);
void			free_int_array(int **array);

t_builtin_fn	get_builtin(char *cmd_name);
int				execute_builtin(t_cmd *cmd, t_env *env,
					int input_fd, int output_fd);

t_env			*env_init(char **envp);
char			*env_get(t_env *env, char *key);
char			*build_string(char *err_string, char *value);
int				env_key_check(char *key, char *value, int plus, int i);
int				env_update(t_env *env, char *key, char *value, int plus);
int				env_set(t_env *env, char *key, char *value, int plus);
int				env_unset(t_env **env, char *key);
void			env_free(t_env *env);

//expansion.c
char			*no_expansion(char **rl, char *token);
char			*expansion(char **rl, t_env *env, char *token, int dq);
char			*expand_var(char **rl, t_env *env);
char			*get_var_name(const char *str);

//parser.c
char			**cmd_parser_rl(char *rl, t_env *env, int *val, int tok_count);
char			**cmd_parser_rl2(char *rl, t_env *env, int *val, int tok_count);
void			cmd_parser(char *rl, t_cmd *cmd, t_env *env);

//parser_extract.c
char			*extract_quoted_token(char **rl, t_env *env);
char			*extract_word(char **rl, t_env *env);
char			*extract_operator(char **rl);

//parser_utils.c
int				check_previuos_op(char *op, char **tokens, int i);
int				count_tokens(char *rl, int count);
char			*skip_quotes(char *rl);

//parser_redirects.c
int				is_operator_start(char c);
t_operator		get_operator_type(const char *op);

char			*cmd_check(t_cmd *cmd, t_env *env);
int				add_redirect(t_cmdblock *block, t_operator type, char *file);

int				cmd_setup(t_cmd *cmd, t_env *env, char ***args,
					char **full_path);
void			cmd_init(char *rl, t_cmd *cmd, t_env *env);
void			cmd_exec_inline(int argc, char **argv, t_env *env, t_cmd *cmd);
int				cmd_exec_update_position_cmd(t_cmd **cmdtmp);
void			cmd_exec_setup_pipe(t_cmdblock *block, int *fd, int *prev_pipe);
int				cmd_exec_handle_redir(t_cmdblock *block,
					int *pipefd, int *fd_output);
int				cmd_exec_handle_heredoc(char *delimiter);
int				cmd_exec_setup_redir(t_redirect *redirects);
int				cmd_exec_setup_heredoc(t_redirect *redirects, int *heredoc_fd);
void			cmd_exec_pipe_cmd(t_cmd *cmd, t_env *env, int infd, int outfd);
void			cmd_exec_pipe_wait_children(int *result);
int				cmd_exec_pipe_check(t_cmd *cmdtmp, int *pipefd);
int				cmd_exec(t_cmd *cmd, t_env *env);
void			cmd_debug(t_cmd *cmd);
void			free_cmd(t_cmd *cmd);

t_cmdblock		*get_first_block(t_cmdblock *block);
int				builtin_echo(t_cmd *cmd, t_env *env);
int				builtin_cd(t_cmd *cmd, t_env *env);
int				builtin_export(t_cmd *cmd, t_env *env);
int				builtin_unset(t_cmd *cmd, t_env *env);
int				builtin_env(t_cmd *cmd, t_env *env);
int				builtin_exit(t_cmd *cmd, t_env *env);
int				builtin_pwd(t_cmd *cmd, t_env *env);

#endif
