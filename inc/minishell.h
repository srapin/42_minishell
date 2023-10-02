/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 22:20:56 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 13:53:17 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
//#include <wait.h>
# include "lib.h"
# include "struct.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <termios.h>
// #include "pub.h"

extern int		g_exit_status;

# define EXIT_SIG_INT = 130;
# define GETCWD_INITIAL_BUF_SIZE 50 /* Initial buffer size for getcwd()'s call*/

// parsing
# define EXIT_OK 0
# define SYNTAX_ERROR 2

// exec
# define NO_FILE_OR_DIR 1 // "no such file or directory" error's exit status
# define CMD_NOT_EXECUTABLE 126
// cannot execute the command (ex : `./` : is a directory)
# define CMD_NOT_FOUND 127

// builtins
# define CD_TOO_MANY_ARGS 1
# define INVALID_VAR_ID 1
# define CANNOT_ACCESS_DIR 1
# define HOME_NOT_SET 1
# define NOT_A_NUM 2

/* -------------------------- SRUCT---------------------------------*/

//t_cmd
void			init_cmd(t_cmd *cmd, char **envp);
void			add_args_to_cmd(t_cmd *cmd, char *args);
void			add_value_to_cmd(t_cmd *cmd, char *val);

//t_file_struct
void			init_file_struct(t_file *file_struct);
void			init_file_struct_with_filename(t_file *file_struct,
					char *filename);
void			init_file_struct_with_sep(t_file *file_struct, char *sep);
void			init_file_struct_with_fd(t_file *file_struct, int fd);
t_file			*create_file_struct_with_filename(char *filename);
t_file			*create_file_struct_with_fd(int fd);

//red
// void			add_in_redir_with_file_struct(t_cmd *cmd, t_file *file_struct);
// void			add_out_redir_with_file_struct(t_cmd *cmd, t_file *file_struct);
// void			add_err_redir_with_file_struct(t_cmd *cmd, t_file *file_struct);
void			add_redir_with_file_struct(t_cmd *cmd, t_file *file_struct);

void			init_redirections(t_redirect *red);
t_redirect		*create_redir(void);

//cmd
t_cmd			*create_cmd_with_val(char **envp, char *args);
int				count_cmds_linked_by_pipe(t_cmd *first_cmd);
int				count_cmds_linked_by_pipe(t_cmd *first_cmd);
void			mask_and_free_for_exec(t_cmd *cmd, t_cmd *first);
/* -------------------------- HASH TABLE ---------------------------*/

t_ht_hash_table	*ht_new(size_t size);
void			ht_resize(t_ht_hash_table *ht, size_t base_size);
void			ht_insert_item(t_ht_hash_table *ht, char *key, char *value);
t_ht_item		*ht_new_item(char *key, char *value);
char			*ht_search(t_ht_hash_table *ht, const char *key);
int				ht_modify_value(t_ht_hash_table *ht, const char *key,
					char *new_value);
void			ht_delete(t_ht_hash_table *ht, const char *key);
void			ht_del_item(t_ht_item *item);
void			ht_del_hash_table(t_ht_hash_table *ht);
int				ft_strcmp(const char *s1, const char *s2);
void			free_tab(char **tab);
size_t			ht_get_hash(const char *key, const size_t num_buckets,
					size_t attempts);

// ------ temporaire : a mettre dans la libft ici

long			ft_pow(long a, long b);
int				next_prime(int nb);
int				is_prime(int nb);

/* ---------------------- TOKENS STREAM -------------------------*/

t_token_list	*tk_new_elem(char *t_stream, size_t t_len, int t_type,
					int is_quote);
void			tk_add(t_token_list **first, t_token_list *new);
void			tk_add_word_in_list(t_token_list **current, char *content);
void			tk_del_one(t_token_list **first, t_token_list *to_del);

/* ------------------------- EXEC -------------------------------*/

void			exec_cmds(t_cmd *cmd);
void			malloc_error(void);
int				(*get_builtins_foo(char *str))(t_cmd *, t_cmd *);

//common_process
void			parent_process(t_cmd **cmd, int pipe_tab[2]);
void			child_process(t_cmd *cmd, t_cmd *first, int pipe_tab[2]);
void			fail_process(void);

// signals
void			sigint_during_cmd_exec(int sig);
void			sigint_next_prompt(int sig);
void			hd_sigint(int i);
void			reset_defaults_signals(void);
//exec_cmd
void			exec_cmds(t_cmd *first_cmd);
int				try_to_exec_builtins(t_cmd *cmd, t_cmd *first, bool is_child);

//next
bool			check_ret(t_cmd *cmd, int ret);
void			wait_childs(t_cmd *cmd);

//safer
void			reset_pip_tab(int pip_tab[2]);
bool			check_acces(t_cmd *cmd, t_cmd *first);
void			safe_close_cmd_fd(t_cmd *cmd);
void			safe_pipe(int pipe_tab[2]);
void			safe_close(int *fd);

//tools
bool			ft_strisequal(char *s1, char *s2);
void			free_tab(char **tab);
void			add_slash(char **paths);
char			**get_path(t_cmd *cmd);
int				is_dir(const char *path);
bool			value_is_not_null(t_cmd *cmd);
bool			look_like_a_cmd(t_cmd *cmd);

void			link_cmds_with_redirections(t_cmd *cmd, t_cmd *next);
void			link_cmds_with_ctrls_op(t_cmd *cmd, t_cmd *next, t_ctrl_op c);

int				count_cmds_linked_by_pipe(t_cmd *first_cmd);
void			safe_pipe(int pipe_tab[2]);
void			fail_process(void);

bool			open_cmd_files(t_cmd *cmd);
void			parent_process(t_cmd **cmd, int pipe_tab[2]);
void			safe_close_cmd_fd(t_cmd *cmd);
bool			dup_cmd_file(t_cmd *cmd);

void			not_found(t_cmd *cmd, t_cmd *first);
void			failed_to_open_files(t_cmd *first);
void			cmd_not_found(t_cmd *cmd, t_cmd *first);
bool			try_to_find_path(t_cmd *cmd);

/* ------------------- PARSING ----------------------*/

void			read_lines(t_data *data);
t_ht_hash_table	*ht_get_env(char **envp, char *exec_name);
t_ht_hash_table	*get_minimal_env(char *exec_name);

t_token			*assign_type(char *input, size_t stream_len);
t_token_list	**tokenise(t_token *token_stream, size_t stream_len,
					char *input);
t_cmd			*parse_and_execute(t_data *data);

int				ft_syntax(t_data *data);
void			display_custom_se(bool quotes);
int				check_syntax(t_data *data);
int				check_pipelines(t_data *data, int *parentheses_count);
int				check_control_op(t_data *data, t_token_list **current);
int				check_simple_command(t_data *data, t_token_list **current,
					int *parentheses_count);
int				check_first(t_data *data, t_token_list **first);
void			display_se(t_data *data, char *token);

void			perform_variable_exp(t_data *data);
int				valid_name(char *name);
int				check_for_exit_status(t_token_list *current,
					char **d_start, char *next_d_start);
void			check_next_token(t_token_list **curr, size_t dollar_index);
void			expand(t_ht_hash_table *ht, t_token_list **current, char *var,
					size_t dollar_index);
void			search_and_expand(t_ht_hash_table *ht, t_token_list **current,
					char *var, size_t dollar_index);
char			*get_var_name(t_token_list *current, char *next_d_start,
					size_t d_index, size_t next_d_index);
void			remove_char(t_token_list *current, size_t index);
int				is_only_dollars(char *str);
char			*get_next_d_start(char *d_start);
void			update_tk_content(t_token_list **current, char *before_key,
					char *value, char *after_value);
void			check_for_empty_content(t_token_list **first,
					t_token_list **current);
int				is_exit_status(t_token_list *current, char *dollar_start);

void			delete_quotes(t_data *data);
void			rm_quotes_token(t_token_list *current);
void			group_words(t_data *data);
void			del_whitespace(t_data *data, t_token_list **current);
t_word_data		*new_word_data(t_token_list *token);
void			add_word_data(t_word_data **first, t_word_data *to_add);

void			perform_wildcard_exp(t_data *data);
void			print_error_wildcard_opendir(char *error, char *dir_name);
t_filename		*first_sort(t_data *data, char *prefix, char *suffix);
void			second_sort(t_filename **filenames, t_token_list *current,
					char *prefix);
int				is_in_quotes(t_token_list *current, size_t index);
void			free_and_null(char *prefix, char *suffix);
void			insert_filenames(t_token_list **first, t_token_list **current,
					t_filename **filenames);
t_filename		*return_entire_dir_content(char *current_dir, DIR *dir);
char			*search_from_end(char *d_name, char *suffix);
char			*search_from_start(char *d_name, char *prefix);
void			add_filename(t_filename **filenames, char *new);
int				is_a_hidden_dir(struct dirent *dir_content);
int				get_next_wcard_index(t_token_list *current, size_t index);
char			*ft_search_str_in_str(char *s, char *to_find);
void			del_filename(t_filename **first, t_filename *to_del);
void			free_filenames(t_filename **filenames);

bool			set_here_docs(t_data *data);
void			get_here_doc_content(t_ht_hash_table *ht, int fd, char *limiter,
					int quotes);
int				set_quotes(t_token_list *current);
void			set_hd_filenames(t_token_list *current, t_data *data);
void			parse_current_tk(t_ht_hash_table *ht, t_token_list **first,
					t_token_list **current);

t_cmd			*get_ast(t_data *data);
void			set_simple_command(t_cmd *current_cmd,
					t_token_list **curr_tk);
void			set_subshell(t_cmd *current_cmd, t_token_list **curr_tk);
int				get_words_count(t_token_list *current);
void			set_command_attributs(t_cmd **current,
					t_token_list *current_tk, int args_count);
int				get_whtsp_pos(char *str, int whtsp_pos);
void			whitespaces_skip_assess(t_word_data *wd, int *wht_pos,
					int *p_wht_pos);
void			skip_whitespaces_reassess_indexes(char *curr_content,
					int *wht_pos, int *p_wht_pos);
void			set_buffer(char **buffer, char *substr);
void			reassess_buffer(t_cmd **curr_cmd, char **buffer, int *i);
int				is_a_t_ctrl_op(t_token_list *current);
void			add_to_cmd_list(t_cmd **last, t_cmd *new);
t_cmd			*init_new_cmd(t_data *data);
void			free_tokens(t_token_list **first);
char			**hash_map_to_tab(t_ht_hash_table *ht);

/* ------------------------- BUILTINS ---------------------------- */

int				ft_echo(t_cmd *cmd, t_cmd *first);
int				ft_env(t_cmd *cmd, t_cmd *first);
int				ft_export(t_cmd *cmd, t_cmd *first);
void			search_and_insert(t_list **head, t_list *elem);
int				ft_unset(t_cmd *cmd, t_cmd *first);
int				is_in_export_history(t_list *export_hist, char *var_name);
void			del_from_export_history(t_list **export_hist, char *var_name);
void			print_export_history(t_ht_hash_table *ht, t_list *export_hist);
int				ft_cd(t_cmd *cmd, t_cmd *first);
char			*get_full_path(t_ht_hash_table *env, char *arg_path);
char			*replace_prev_or_actual_dir(char *path);
void			set_path(char **path, char *before, char *after);
void			curr_dir(char **path, int i);
int				ft_pwd(t_cmd *cmd, t_cmd *first);
char			**get_pwd(t_ht_hash_table *env);
void			update_pwd(t_ht_hash_table *env, char *new_pwd);
int				ft_exit(t_cmd *cmd, t_cmd *first);

/* --------------------- FREE DATA -----------------------------*/

void			free_parsing_data(t_data *data);
void			free_pwd(t_ht_hash_table *env);
void			free_cmd(t_cmd **cmd);
void			free_cmds(t_cmd **cmd, bool common);
void			ft_lstfree(t_list **lst, void free_foo(void *));
void			free_merged_words(t_word_data *wd);
void			call_free_fs(void *p);
void			free_red(t_redirect *red);
void			free_file_struct(t_file *file_struct);
void			free_cmd_value(t_cmd_value *val);

// to del
void			print_ht(t_ht_hash_table *ht);
void			print_tokens(t_token_list *first);
void			print_ast(t_cmd *ast);

#endif