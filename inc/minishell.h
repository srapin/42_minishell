/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:27:43 by srapin            #+#    #+#             */
/*   Updated: 2023/06/13 01:44:54 by srapin           ###   ########.fr       */
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
#include <dirent.h> /* For readdir() */
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
//#include <wait.h>
#include <readline/readline.h>
#include <readline/history.h>

# include "lib.h"
#include "struct.h"
#include "pub.h"

#include "hash_table.h"
#include "defines.h"
#include "tokens.h"

extern int g_exit_status;

void exec_cmds(t_cmd *cmd);

int (*get_builtins_foo(char *str))(t_cmd *);
//common_process
void	parent_process(t_cmd **cmd, int pipe_tab[2]);
void	child_process(t_cmd *cmd, int pipe_tab[2]);
void	fail_process(void);

void handle_sigint(int sig);
void handle_sigint2(int sig);
void handle_sigquit(int sig);
void handle_sigquit2(int sig);
//exec_cmd
void exec_cmds(t_cmd *first_cmd); //to div

int try_to_exec_builtins(t_cmd *cmd, bool is_child);

//heredoc
void heredoc(t_cmd *cmd);

//next
bool check_ret(t_cmd *cmd, int ret);
int	wait_childs(t_cmd *cmd);

//safer
void reset_pip_tab(int pip_tab[2]);
bool	check_acces(char **paths, t_cmd *cmd);
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


int count_cmds(t_cmd *first_cmd);
void	safe_pipe(int pipe_tab[2]);
void	fail_process(void);
void	safe_close(int *fd);
//to test
void open_cmd_files(t_cmd * cmd);
void set_fd(t_file * f_struct, int *fd, int flag);
void	parent_process(t_cmd **cmd, int pipe_tab[2]);
void safe_close_cmd_fd(t_cmd *cmd);
void dup_cmd_file(t_cmd *cmd);


// ------------ Parsing ------------------

void            read_lines(t_ht_hash_table *ht);

t_token         *assign_type(char *input, size_t stream_len);
t_token_list    *tokenise(t_ht_hash_table *ht, t_token *token_stream, size_t stream_len, char *input);

void            parse(t_ht_hash_table *ht, t_token_list *first);
int             ft_syntax(t_token_list **first);
void            check_syntax(t_token_list **first);

void            perform_variable_exp(t_ht_hash_table *ht, t_token_list **first);
void            expand(t_ht_hash_table *ht, t_token_list **current, char *var, size_t dollar_index);
void            delete_quotes(t_token_list **first);
void            group_words(t_token_list **first);
void            perform_wildcard_exp(t_ht_hash_table *ht, t_token_list **first);

int             is_in_quotes(t_token_list *current, size_t index);

void            remove_char(t_token_list *current, size_t index);

void            set_here_docs(t_ht_hash_table *ht, t_token_list **first);

t_cmd           *get_ast(t_ht_hash_table *ht, t_token_list **first_tk);
t_list          *init_export_history(t_ht_hash_table *ht);

void            free_tokens(t_token_list **first);

char            **hash_map_to_tab(t_ht_hash_table *ht);

void            print_tokens(t_token_list *first);
void            print_ast(t_cmd *ast);


// builtins
int             ft_env(t_cmd *cmd);
int             ft_export(t_cmd *cmd);
int             ft_unset(t_cmd *cmd);
int             is_in_export_history(t_list *export_hist, char *var_name);
void            del_from_export_history(t_list **export_hist, char *var_name);

int             ft_cd(t_cmd *cmd);
int             ft_pwd(t_cmd *cmd);
int             ft_exit(t_cmd *cmd);


// to del
//void replace_prev_or_actual_dir(char *path);

#endif