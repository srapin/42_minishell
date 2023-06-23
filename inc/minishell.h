/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapi <rapi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:27:43 by srapin            #+#    #+#             */
/*   Updated: 2023/06/20 01:33:55 brapi agiguair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
//#include <wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <termios.h>

# include "lib.h"
#include "struct.h"
#include "pub.h"

//#include "hash_table.h"
//#include "defines.h"
//#include "tokens.h"

extern int g_exit_status;
#define EXIT_SIG_INT = 130;
# define GETCWD_INITIAL_BUF_SIZE    50 /* Initial buffer size for getcwd()'s call*/

// parsing
# define EXIT_OK                    0
# define SYNTAX_ERROR               2

// exec
# define NO_FILE_OR_DIR             1 // "no such file or directory" error's exit status
# define CMD_NOT_EXECUTABLE         126 // cannot execute the command (ex : `./` : is a directory)
# define CMD_NOT_FOUND              127

// builtins
# define CD_TOO_MANY_ARGS           1
# define INVALID_VAR_ID             1 // "not a valid identifier"'s export error message
# define CANNOT_ACCESS_DIR          1 // chdir error 
# define HOME_NOT_SET               1
# define NOT_A_NUM                  2 // "numeric argument required" error in exit()


/* -------------------------- HASH TABLE ---------------------------*/

t_ht_hash_table     *ht_new(size_t size);
void                ht_resize(t_ht_hash_table *ht, size_t base_size);
void                ht_insert_item(t_ht_hash_table *ht, char *key, char *value);
t_ht_item           *ht_new_item(char *key, char *value);
char                *ht_search(t_ht_hash_table *ht, const char *key);
int                 ht_modify_value(t_ht_hash_table *ht, const char *key, char *new_value);
void                ht_delete(t_ht_hash_table *ht, const char *key);
void                ht_del_item(t_ht_item *item);
void                ht_del_hash_table(t_ht_hash_table *ht);
int                 ft_strcmp(const char *s1, const char *s2);
void                free_tab(char **tab);
size_t              ht_get_hash(const char *key, const size_t num_buckets, size_t attempts);

// ------ temporaire : a mettre dans la libft ici

long    ft_pow(long a, long b);
int	next_prime(int nb);
int	is_prime(int nb);

/* ---------------------- TOKENS STREAM -------------------------*/

t_token_list    *tk_new_elem(char *t_stream, size_t t_len, int t_type, int is_quote);
void            tk_add(t_token_list **first, t_token_list *new); // add at the end
void            tk_add_word_in_list(t_token_list **current, char *content); // add after the current
void            tk_del_one(t_token_list **first, t_token_list *to_del);

/* ------------------------- EXEC -------------------------------*/

void    exec_cmds(t_cmd *cmd);
void    malloc_error();
int     (*get_builtins_foo(char *str))(t_cmd *, t_cmd *);
//common_process
void	parent_process(t_cmd **cmd, int pipe_tab[2]);
void	child_process(t_cmd *cmd, t_cmd *first, int pipe_tab[2]);
void	fail_process(void);

void    sigint_during_cmd_exec(int sig);
void    sigint_next_prompt(int sig);
// void handle_sigquit(int sig);
// void handle_sigquit2(int sig);
//exec_cmd
void    exec_cmds(t_cmd *first_cmd); //to div

int     try_to_exec_builtins(t_cmd *cmd, t_cmd *first, bool is_child);

//heredoc
// void heredoc(t_cmd *cmd)t_token_list

//next
bool    check_ret(t_cmd *cmd, int ret);
void	wait_childs(t_cmd *cmd);

//safer
void reset_pip_tab(int pip_tab[2]);
bool	check_acces(t_cmd *cmd);
void safe_close_cmd_fd(t_cmd *cmd);
void	safe_pipe(int pipe_tab[2]);
void	safe_close(int *fd);

//tools
bool	ft_strisequal(char *s1, char *s2);
void	free_tab(char **tab);
void	add_slash(char **paths);
char	**get_path(t_cmd *cmd);
//complete
void link_cmds_with_redirections(t_cmd *cmd, t_cmd *next);
void link_cmds_with_ctrls_op(t_cmd *cmd, t_cmd *next, ctrl_op c);


int count_cmds_linked_by_pipe(t_cmd *first_cmd);
void	safe_pipe(int pipe_tab[2]);
void	fail_process(void);
void	safe_close(int *fd);
//to test
bool open_cmd_files(t_cmd * cmd);
void	parent_process(t_cmd **cmd, int pipe_tab[2]);
void safe_close_cmd_fd(t_cmd *cmd);
bool dup_cmd_file(t_cmd *cmd);


// ------------ Parsing ------------------

void            read_lines(t_data *data);
t_ht_hash_table *ht_get_env(char **envp);
t_ht_hash_table *get_minimal_env(void);

t_token         *assign_type(char *input, size_t stream_len);
t_token_list    **tokenise(t_token *token_stream, size_t stream_len, char *input);
t_cmd           *parse_and_execute(t_data *data);
int             ft_syntax(t_data *data);
void            check_syntax(t_data *data);

void            perform_variable_exp(t_data *data);
int             valid_name(char *name);
void            expand(t_ht_hash_table *ht, t_token_list **current, char *var, size_t dollar_index);
void            delete_quotes(t_data *data);
void            group_words(t_data *data);
void            perform_wildcard_exp(t_data *data);
t_filename      *first_sort(DIR *dir, char *prefix, char *suffix);
void            second_sort(t_filename **filenames, t_token_list *current, char *prefix);
int             is_in_quotes(t_token_list *current, size_t index);
void            remove_char(t_token_list *current, size_t index);

bool            set_here_docs(t_data *data);

t_cmd           *get_ast(t_data *data);
t_list          *init_export_history(t_ht_hash_table *ht);
void            free_tokens(t_token_list **first);
char            **hash_map_to_tab(t_ht_hash_table *ht);

// builtins
int             ft_env(t_cmd *cmd, t_cmd *first);
int             ft_export(t_cmd *cmd, t_cmd *first);
int             ft_unset(t_cmd *cmd, t_cmd *first);
int             is_in_export_history(t_list *export_hist, char *var_name);
void            del_from_export_history(t_list **export_hist, char *var_name);
int             ft_cd(t_cmd *cmd, t_cmd *first);
int             ft_pwd(t_cmd *cmd, t_cmd *first);
char            **get_pwd(t_ht_hash_table *env);
void            update_pwd(t_ht_hash_table *env, char *new_pwd);
int             ft_exit(t_cmd *cmd, t_cmd *first);


void            free_parsing_data(t_data *data);
void            free_pwd(t_ht_hash_table *env);
void            free_cmd(t_cmd **cmd);
void            free_cmds(t_cmd **cmd, bool common);
void	        ft_lstfree(t_list **lst,void free_foo(void *));


// to del
void        print_ht(t_ht_hash_table *ht);
void            print_tokens(t_token_list *first);
void            print_ast(t_cmd *ast);

#endif