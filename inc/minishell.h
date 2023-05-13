/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:27:43 by srapin            #+#    #+#             */
/*   Updated: 2023/05/13 02:03:58 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdbool.h>
# include "../libft/ft_printf.h"
# include "../libft/gnl/get_next_line_bonus.h"
# include "../libft/mylibft/libft.h"

typedef enum type_parse
{
	pip,
	literal,
	cmd
}					type_parse;

typedef enum ctrl_op
{
	none,
	and,
	or
	,
	pointvirgule
}					ctrl_op;

typedef enum type_redirect
{
	noneu,
	fd,
	cmds_node,
	pipeu
}					type_redirect;

typedef struct cmd_value
{
	//int child_count;
	char			*value;
	char			*path;
}					t_cmd_value;

typedef struct file
{
	char *name; //dans parsing
	int fd;     //si deja ouvert. dans parsing -> mettre a -1
	char *sep;  //mettre a null si pas heredoc
	int flag;
	/* data */
}					t_file;

typedef struct redirect
{
	type_redirect	in_type;
	int				in_fd;
	void *in_content; // soit un t_node soit un truc
	type_redirect	out_type;
	int				out_fd;
	void *out_content; // soit un t_node soit un truc
	type_redirect	err_type;
	int				err_fd;
	void *err_content; // soit un t_node soit un truc
}					t_redirect;

typedef struct node
{
	//type_parse type;
	t_cmd_value val; // nom de la commande
	char			**args;
	t_redirect  red; //null terminated
	char			**env;
	ctrl_op			ctrl;
	struct node		*next;

}					t_node;

void exec_cmds(t_node *node);


//complete
void init_node(t_node *node, char **envp);
void init_redirections(t_redirect *red);
void init_file_struct_with_fd(t_file *file_struct, int fd);
void init_file_struct_with_filename(t_file *file_struct, char *filename);
void init_file_struct(t_file *file_struct);
void add_err_redir_with_file_struct(t_node *node, t_file *file_struct);
void add_out_redir_with_file_struct(t_node *node, t_file *file_struct);
void add_in_redir_with_file_struct(t_node *node, t_file *file_struct);
void link_nodes_with_redirections(t_node *node, t_node *next);
void link_nodes_with_ctrls_op(t_node *node, t_node *next, ctrl_op c);
void add_args_to_node(t_node *node, char *args);
void add_value_to_node(t_node *node, char *val);
int count_cmds(t_node *first_node);
void	safe_pipe(int pipe_tab[2]);
void	fail_process(void);
void	safe_close(int *fd);
//to test
void open_node_files(t_node * node);
void set_fd(t_file * f_struct, int *fd, int flag);
void	parent_process(t_node **node, int pipe_tab[2]);
void safe_close_node_fd(t_node *node);
void dup_node_file(t_node *node);

//todo

#endif