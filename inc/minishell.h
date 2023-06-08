/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:27:43 by srapin            #+#    #+#             */
/*   Updated: 2023/06/08 15:03:50 by Helene           ###   ########.fr       */
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
//#include <wait.h>
#include <readline/readline.h>
#include <readline/history.h>

# include "lib.h"
#include "struct.h"
#include "pub.h"

#include "hash_table.h"
#include "defines.h"
#include "tokens.h"


void exec_cmds(t_cmd *cmd);


//common_process
void	parent_process(t_cmd **cmd, int pipe_tab[2]);
void	child_process(t_cmd *cmd, int pipe_tab[2], int *to_free);
void	fail_process(void);

//exec_cmd
void exec_cmds(t_cmd *first_cmd); //to div


//heredoc
void heredoc(t_cmd *cmd);

//next
bool check_ret(t_cmd *cmd, int ret);
int	wait_childs(int size, int *childs_pid);

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
char	**get_path(char **envp);
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
void            check_pipeline_list(t_token_list **first);

void            perform_variable_exp(t_ht_hash_table *ht, t_token_list **first);
void            expand(t_ht_hash_table *ht, t_token_list **current, char *var, size_t dollar_index);
void            delete_quotes(t_token_list **first);
void            group_words(t_token_list **first);
void            perform_wildcard_exp(t_ht_hash_table *ht, t_token_list **first);

int             is_in_quotes(t_token_list *current, size_t index);

void            remove_char(t_token_list *current, size_t index);

void            assess_and_free(t_file *content);
void            set_here_docs(t_ht_hash_table *ht, t_token_list **first);

void            print_tokens(t_token_list *first);


//todo

#endif