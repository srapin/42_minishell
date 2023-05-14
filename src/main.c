/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:28:06 by srapin            #+#    #+#             */
/*   Updated: 2023/05/13 04:52:45 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void add_value_to_node(t_node *node, char *val)
{
	node->val.value = val;
}

void add_args_to_node(t_node *node, char *args)
{
	node->args = ft_split(args, ' ');
}

void link_nodes_with_ctrls_op(t_node *node, t_node *next, ctrl_op c)
{
	if (!c)
		perror("bad ctrl add");
	node->ctrl = c;
	node->next = next;
}

void link_nodes_with_redirections(t_node *node, t_node *next)
{
	node->red.out_type = cmds_node;
	node->red.out_content = next;
}



void add_in_redir_with_file_struct(t_node *node, t_file *file_struct)
{
	node->red.in_type = fd;
	node->red.in_content = file_struct;
	if (file_struct->fd > -1)
		node->red.in_fd = file_struct->fd;
}

void add_out_redir_with_file_struct(t_node *node, t_file *file_struct)
{
	node->red.out_type = fd;
	node->red.out_content = file_struct;
	if (file_struct->fd > -1)
		node->red.out_fd = file_struct->fd;
}

void add_err_redir_with_file_struct(t_node *node, t_file *file_struct)
{
	node->red.err_type = fd;
	node->red.err_content = file_struct;
	if (file_struct->fd > -1)
		node->red.err_fd = file_struct->fd;
}

void init_file_struct(t_file *file_struct)
{
	file_struct->name = NULL;
	file_struct->fd = -1;
	file_struct->sep = NULL;
}

void init_file_struct_with_filename(t_file *file_struct, char *filename)
{
	init_file_struct(file_struct);
	file_struct->name = filename;
}

void init_file_struct_with_sep(t_file *file_struct, char *sep)
{
	init_file_struct(file_struct);
	file_struct->sep = sep;
}

void init_file_struct_with_fd(t_file *file_struct, int fd)
{
	init_file_struct(file_struct);
	file_struct->fd = fd;
}

void init_redirections(t_redirect *red)
{
	red->in_type = noneu;
	red->in_fd = -1;
	red->in_content = NULL;
	
	red->out_type = noneu;
	red->out_fd = -1;
	red->out_content = NULL;
	
	red->err_type = noneu;
	red->err_fd = -1;
	red->err_content = NULL;
}

void init_node(t_node *node, char **envp)
{
	add_value_to_node(node, NULL);
	add_args_to_node(node, NULL);
	init_redirections(&(node->red));
	node->env = envp;
	node->ctrl = none;	
	node->next = NULL;
}


void make_cat_node(t_node *node, t_node *ls_node,t_file *f, char **envp)
{
	init_file_struct_with_filename(f, "out");
	
	
	init_node(node, envp);
	add_value_to_node(node, "cat");
	add_in_redir_with_file_struct(node, f);
	
	link_nodes_with_ctrls_op(node, ls_node, and);

}

void make_echo_node(t_node *node, char ** envp)
{
	init_node(node, envp);
	add_value_to_node(node, "echo");
	add_args_to_node(node, "echo hello world");
}

void make_wc_node(t_node * node, t_file *f_s, char ** envp)
{
	init_node(node, envp);
	
	// add_value_to_node(node, "wc");
	// add_args_to_node(node, "wc -l");
	add_value_to_node(node, "cat");
	add_args_to_node(node, "cat");
	//init_file_struct_with_filename(f_s, "out");
	//add_out_redir_with_file_struct(node, f_s);
	
}



void make_grep_node(t_node * node, t_node * wc_node, char ** envp)
{
	init_node(node, envp);
	
	// add_value_to_node(node, "grep");
	// add_args_to_node(node, "grep c");
	add_value_to_node(node, "cat");
	add_args_to_node(node, "cat");
	link_nodes_with_redirections(node, wc_node);
}

void make_ls_node(t_node * node, t_node * grep_node,t_file *f_s,  char ** envp)
{
	init_node(node, envp);
	
	add_value_to_node(node, "cat");
	add_args_to_node(node, "cat");
	link_nodes_with_redirections(node, grep_node);
	// init_file_struct_with_filename(f_s, "in");
	init_file_struct_with_sep(f_s, "in");
	add_in_redir_with_file_struct(node, f_s);
}


int main(int ac, char **av, char **envp)
{
	
	t_file cat_f;
	t_file wc_f;
	t_file ls_f;
	
	t_node cat_node;
	t_node echo_node;
	t_node ls_node;
	t_node grep_node;
	t_node wc_node;
	
	make_cat_node(&cat_node, &ls_node, &cat_f, envp);
	make_echo_node(&echo_node, envp);
	
	make_wc_node(&wc_node, &wc_f, envp);
	
	make_grep_node(&grep_node, &wc_node, envp);
	make_ls_node(&ls_node, &grep_node, &ls_f, envp);
	link_nodes_with_ctrls_op(&cat_node, &ls_node, or);
	link_nodes_with_ctrls_op(&ls_node, &echo_node, and);

	exec_cmds(&ls_node);
	return 0;
}


//    cat out && (in < ls |grep "c" | wc -l > out) || echo "hello world"
 

//    <in >out ls | wc -l >out2 2>out.err | cat out >out3 && echo coucou || cat out