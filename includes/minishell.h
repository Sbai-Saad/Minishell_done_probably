/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasbai <sasbai@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:04:15 by sasbai            #+#    #+#             */
/*   Updated: 2025/08/17 10:04:15 by sasbai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>

# define PROMPT "minishell> "
# define ERROR 1
# define SUCCESS 0
# ifndef ECHOCTL
#  define ECHOCTL 0
# endif

typedef struct s_env			t_env;
typedef struct s_token			t_token;
typedef struct s_cmd			t_cmd;
typedef struct s_shell			t_shell;
typedef struct s_outredir		t_outredir;
typedef struct s_inredir		t_inredir;
typedef struct s_heredoc		t_heredoc;
typedef struct s_token_range	t_token_range;

extern int						g_signal_received;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

struct s_token
{
	char			*value;
	t_token_type	type;
	t_token			*next;
};

struct s_env
{
	char	*key;
	char	*value;
	t_env	*next;
};

struct s_outredir
{
	char		*filename;
	int			append;
	t_outredir	*next;
};

struct s_inredir
{
	char		*filename;
	t_inredir	*next;
};

struct s_cmd
{
	char		**args;
	char		*infile;
	int			heredoc_fd;
	t_heredoc	*heredocs;
	t_inredir	*in_redirs;
	t_outredir	*out_redirs;
	t_cmd		*next;
};

struct s_heredoc
{
	char		*eof;
	int			quoted;
	t_heredoc	*next;
};

struct s_shell
{
	t_env	*env_list;
	t_token	*tokens;
	t_cmd	*cmds;
	int		stdin_copy;
	int		stdout_copy;
	int		exit_status;
	int		running;
};

struct s_token_range
{
	t_token	*start;
	t_token	*end;
};

struct s_buf
{
	char	*data;
	size_t	len;
	size_t	cap;
};

int			buf_init(struct s_buf *b, size_t cap_hint);
int			buf_grow(struct s_buf *b, size_t min_extra);
int			buf_putn(struct s_buf *b, const char *s, size_t n);
void		process_input_line(t_shell *shell, char *input,
				int add_history_flag);
int			hd_expand_eof(struct s_buf *out, const char *p, size_t *adv);
int			hd_expand_qmark(struct s_buf *out, t_shell *shell, size_t *adv);
int			hd_expand_var(struct s_buf *out, const char *q,
				t_shell *shell, size_t *adv);
int			hd_expand_literal_dollar(struct s_buf *out, const char *p,
				size_t *adv);
int			advance_over_field(const char *s, int *i, int *in_sq, int *in_dq);
void		skip_ws_outside_quotes(const char *s, int *i, int in_sq, int in_dq);
void		skip_ws_outside_quotes2(const char *s, int *i,
				int in_sq, int in_dq);
void		advance_over_field2(const char *s, int *i, int *in_sq, int *in_dq);
int			count_unquoted_fields2(const char *s);
char		*extract_raw_field2(const char *s, int *i, int *in_sq, int *in_dq);
void		split_unquoted_and_fill(const char *s, char **args, int *idx_ptr);
t_cmd		*handle_create_failure_and_null(t_shell *shell, t_cmd *cmds);
int			setup_stdin_from_in_redirs(t_inredir *list);
int			redirect_input(char *filename);
int			open_output_fd(const char *filename, int append);
void		init_shell(t_shell *shell, char **envp);

void		init_shell(t_shell *shell, char **envp);
void		run_shell(t_shell *shell);
void		cleanup_shell(t_shell *shell);
t_token		*tokenize_input(char *input);
t_token		*create_token(char *value, t_token_type type);
int			try_expand_at(struct s_buf *out, const char *p,
				t_shell *shell, size_t *adv);
int			resolve_redir_target(const char *word, t_shell *shell,
				char **out_name);
void		add_token(t_token **head, t_token *new_token);
void		free_tokens(t_token *tokens);
int			is_whitespace(char c);
int			is_special_char(char c);
void		process_special_char(char *input, int *i, t_token **tokens);
char		*extract_word(char *input, int *i);
void		exit_command_not_found(t_cmd *cmd);
void		heredoc_setup_signals(void);
int			heredoc_write_line(int wfd, char *line, int quoted, t_shell *shell);
int			parse_input(t_shell *shell, char *input);
t_cmd		*parse_commands(t_token *tokens, t_shell *shell);
void		free_commands(t_cmd *cmds);
t_cmd		*create_command(t_token **tokens, t_token *end, t_shell *shell);
void		init_cmd(t_cmd *cmd);
void		find_pipe_token(t_token **pipe_token, t_token *token_ptr);
int			is_valid_syntax(t_token *tokens);
int			is_redir_token(t_token *cur);
char		**extract_args(t_token **tokens, t_shell *shell);
char		*remove_quotes(char *str);
void		add_command_to_list(t_cmd **cmds, t_cmd *new_cmd);

void		add_outredir(t_cmd *cmd, const char *filename, int append);
void		free_out_redirs(t_outredir *list);

void		add_inredir(t_cmd *cmd, const char *filename);
void		free_in_redirs(t_inredir *list);

char		*expand_variables(char *str, t_shell *shell);

int			is_valid_var_char(char c);
char		*handle_dollar_special(const char **s_ptr, t_shell *shell);
char		*handle_no_var_name(const char **s_ptr, const char *s);
char		*get_var_value(const char **s_ptr, t_shell *shell);
void		append_sanitized(char **pptr, const char *val);
int			try_handle_quote(const char **current, char **ptr,
				int *in_sq, int *in_dq);
int			try_handle_dollar(const char **current, char **ptr,
				int in_sq, t_shell *shell);

int			count_unquoted_fields(const char *s);
char		*extract_next_unquoted_field(const char *s,
				int *i, int *in_sq, int *in_dq);
char		**split_unquoted_to_array(const char *s, int *out_count);
int			has_quotes(const char *s);
int			choose_redir_name(const char *word, char *expanded,
				t_shell *shell, char **out_name);
int			choose_redir_name_single(const char *word, char *expanded,
				t_shell *shell, char **out_name);
int			handle_in_redirection(t_cmd *cmd, t_token *token, t_shell *shell);
int			handle_out_or_append_redirection(t_cmd *cmd, t_token *token,
				t_shell *shell);
int			handle_file_redirection(t_cmd *cmd, t_token *token, t_shell *shell);
int			handle_heredoc_redirection(t_cmd *cmd, t_token *token);
int			process_redirections(t_cmd *cmd, t_token **current, t_shell *shell);
int			collect_cmd_body(t_cmd *cmd, struct s_token_range range,
				t_shell *shell, t_token **out_temp_head);
int			process_or_collect_token(t_cmd *cmd, t_token **current,
				t_shell *shell, t_token **temp_head);

int			execute_commands(t_shell *shell);
int			execute_single_command(t_shell *shell, t_cmd *cmd);
int			execute_pipeline(t_shell *shell, t_cmd *cmd);
char		*find_command_path(char *cmd, t_env *env);
int			handle_fork_error(pid_t pid);
int			wait_all_pipes(int last_pid, int *any_sigint, int *any_sigquit);
void		child_exec_fail(t_shell *shell, int status);
void		exec_child_fail(int saved_errno, t_shell *shell);
char		*resolve_path_or_exit(t_shell *shell, t_cmd *cmd);
void		exec_external_or_exit(t_shell *shell, t_cmd *cmd, char *path);
void		exec_child_pipeline(t_shell *shell, t_cmd *cmd);
int			open_next_pipe(int has_next, int pipefd[2], int prev_fd);
int			fork_failed_cleanup(int has_next, int pipefd[2], int prev_fd);
void		child_dup_and_close(int prev_fd, int has_next, int pipefd[2]);
void		parent_close_and_shift(int *prev_fd, int has_next, int pipefd[2]);
int			spawn_one(t_shell *shell, t_cmd *cmd, int *prev_fd, pid_t *out_pid);
void		post_wait_prints(int any_sigint, int any_sigquit);
int			pipeline_spawn_all(t_shell *shell, t_cmd *cmd, int *prev_fd,
				pid_t *last_pid);
int			pipeline_finalize(t_shell *shell, pid_t last_pid);

int			handle_redirections(t_cmd *cmd);
void		restore_redirections(int stdin_copy, int stdout_copy);
void		handle_heredocs(t_cmd *cmds, t_shell *shell);
char		*expand_heredoc_line(const char *s, t_shell *shell);
char		*search_in_paths(char *cmd, char **paths);
int			is_exec_file(const char *path);
void		set_signal_flags(int status, int *any_sigint, int *any_sigquit);
int			compute_last_status(int status, int last_status,
				pid_t pid, int last_pid);
int			wait_loop(int last_pid, int *any_sigint, int *any_sigquit);

int			is_builtin(char *cmd);
int			execute_builtin(t_shell *shell, t_cmd *cmd);
int			builtin_echo(t_cmd *cmd);
int			builtin_cd(t_shell *shell, t_cmd *cmd);
int			builtin_pwd(t_shell *shell);

int			builtin_export(t_shell *shell, t_cmd *cmd);
int			builtin_unset(t_shell *shell, t_cmd *cmd);
int			builtin_env(t_shell *shell);
int			builtin_exit(t_shell *shell, t_cmd *cmd);
void		print_sorted_env(t_env *env);
void		sort_env_array(t_env **env_array, int count);
void		parse_env_var(char *arg, char **key, char **value, int *append);

t_env		*init_env(char **envp);
void		free_env(t_env *env);
char		*get_env_value(t_env *env, char *key);
void		set_env_value(t_env **env_list, char *key, char *value);
char		**env_to_array(t_env *env);
void		free_env_array(char **env_array);

void		*ft_malloc(size_t size);
void		ft_error(const char *msg);
void		ft_perror(const char *prefix);
char		*ft_strdup(const char *s);
size_t		ft_strlen(const char *s);
char		*ft_strndup(const char *s, size_t n);
void		ft_free_arr(char **arr);
char		*ft_strjoin(const char *s1, const char *s2);
char		**ft_split(char const *s, char c);
char		*ft_strchr(const char *s, int c);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_strcmp(const char *s1, const char *s2);
void		*ft_memcpy(void *dst, const void *src, size_t n);
char		*ft_itoa(int n);
char		*ft_strcpy(char *dest, const char *src);
int			ft_isaalpha(char *s);
int			ft_isnum(char *s);
t_cmd		*create_next_command(t_token **current_token,
				t_token **pipe_token, t_shell *shell);
void		heredoc_child(int wfd, char *delimiter,
				int quoted, t_shell *shell);
int			heredoc_wait_and_result(pid_t pid, int rfd);

void		trim_spaces(const char **s_ptr, const char **end_ptr);
int			is_str_numeric(const char *s);
int			accumulate_value(const char *s, size_t len,
				unsigned long long *out_val);

void		setup_signals(void);
void		reset_signals(void);
void		handle_sigint(int sig);
void		handle_sigquit(int sig);
void		ignore_signals(void);
void		set_echoctl(int enable);
void		parent_post_wait_status(t_shell *shell, int status);
int			run_builtin_in_parent(t_shell *shell, t_cmd *cmd);
int			parent_dup_stdio(t_shell *shell);
void		child_try_execve(t_shell *shell, t_cmd *cmd, char *path);
char		*resolve_path_for_child(t_shell *shell, t_cmd *cmd);
void		exit_not_found_with_path_unset(t_cmd *cmd);
void		ensure_not_directory(char *path, t_cmd *cmd);
void		execute_child_process(t_shell *shell, t_cmd *cmd);
#endif