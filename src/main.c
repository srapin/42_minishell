/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:28:06 by srapin            #+#    #+#             */
/*   Updated: 2023/05/21 23:14:28 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void add_value_to_cmd(t_cmd *cmd, char *val) //do not use!
{
	cmd->val.value = val;
}

void add_args_to_cmd(t_cmd *cmd, char *args)//do not use!
{
	cmd->val.args = ft_split(args, ' ');
}

void add_cmdval_to_cmd(t_cmd *cmd, char *str)
{
	add_args_to_cmd(cmd, str);
	if (cmd->args)
		add_value_to_cmd(cmd, cmd->args[0]);
}

void link_cmds_with_ctrls_op(t_cmd *cmd, t_cmd *next, ctrl_op c)
{
	if (!c)
		perror("bad ctrl add");
	cmd->ctrl = c;
	cmd->next = next;
}

void link_cmds_with_redirections(t_cmd *cmd, t_cmd *next)
{
	cmd->red.out_type = cmds_cmd;
	cmd->red.out_content = next;
}

void add_in_redir_with_file_struct(t_cmd *cmd, t_file *file_struct)
{
	cmd->red.in_type = fd;
	cmd->red.in_content = file_struct;
	if (file_struct->fd > -1)
		cmd->red.in_fd = file_struct->fd;
}

void add_out_redir_with_file_struct(t_cmd *cmd, t_file *file_struct)
{
	cmd->red.out_type = fd;
	cmd->red.out_content = file_struct;
	if (file_struct->fd > -1)
		cmd->red.out_fd = file_struct->fd;
}

void add_err_redir_with_file_struct(t_cmd *cmd, t_file *file_struct)
{
	cmd->red.err_type = fd;
	cmd->red.err_content = file_struct;
	if (file_struct->fd > -1)
		cmd->red.err_fd = file_struct->fd;
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

void init_cmd(t_cmd *cmd, char **envp)
{
	add_value_to_cmd(cmd, NULL);
	add_args_to_cmd(cmd, NULL);
	init_redirections(&(cmd->red));
	cmd->env = envp;
	cmd->ctrl = none;	
	cmd->next = NULL;
}



void init_bag(t_bag *bag)
{
	bag->content = NULL;
	bag->next = NULL;
	bag->type = none;
	bag->ctrl =none;
	bag->ret = -1;
	bag->already_exec = false;
}

void make_cat_cmd(t_cmd *cmd, t_cmd *ls_cmd,t_file *f, char **envp)
{
	init_file_struct_with_filename(f, "out");
	
	
	init_cmd(cmd, envp);
	add_value_to_cmd(cmd, "cat");
	add_in_redir_with_file_struct(cmd, f);
	
	link_cmds_with_ctrls_op(cmd, ls_cmd, and);

}

void make_echo_cmd(t_cmd *cmd, char ** envp)
{
	init_cmd(cmd, envp);
	add_value_to_cmd(cmd, "echo");
	add_args_to_cmd(cmd, "echo hello world");
}

void make_wc_cmd(t_cmd * cmd, t_file *f_s, char ** envp)
{
	init_cmd(cmd, envp);
	
	// add_value_to_cmd(cmd, "wc");
	// add_args_to_cmd(cmd, "wc -l");
	add_value_to_cmd(cmd, "cat");
	add_args_to_cmd(cmd, "cat");
	init_file_struct_with_filename(f_s, "out");
	add_out_redir_with_file_struct(cmd, f_s);
	
}



void make_grep_cmd(t_cmd * cmd, t_cmd * wc_cmd, char ** envp)
{
	init_cmd(cmd, envp);
	
	// add_value_to_cmd(cmd, "grep");
	// add_args_to_cmd(cmd, "grep c");
	add_value_to_cmd(cmd, "cat");
	add_args_to_cmd(cmd, "cat");
	link_cmds_with_redirections(cmd, wc_cmd);
}

void make_ls_cmd(t_cmd * cmd, t_cmd * grep_cmd,t_file *f_s,  char ** envp)
{
	init_cmd(cmd, envp);
	
	add_value_to_cmd(cmd, "cat");
	add_args_to_cmd(cmd, "cat");
	link_cmds_with_redirections(cmd, grep_cmd);
	// init_file_struct_with_filename(f_s, "in");
	init_file_struct_with_sep(f_s, "in");
	add_in_redir_with_file_struct(cmd, f_s);
}


int main(int ac, char **av, char **envp)
{
	
	t_file cat_f;
	t_file wc_f;
	t_file ls_f;
	
	t_cmd cat_cmd;
	t_cmd echo_cmd;
	t_cmd ls_cmd;
	t_cmd grep_cmd;
	t_cmd wc_cmd;
	
	make_cat_cmd(&cat_cmd, &ls_cmd, &cat_f, envp);
	make_echo_cmd(&echo_cmd, envp);
	
	make_wc_cmd(&wc_cmd, &wc_f, envp);
	
	make_grep_cmd(&grep_cmd, &wc_cmd, envp);
	make_ls_cmd(&ls_cmd, &grep_cmd, &ls_f, envp);
	link_cmds_with_ctrls_op(&cat_cmd, &ls_cmd, or);
	link_cmds_with_ctrls_op(&ls_cmd, &echo_cmd, and);

	exec_cmds(&ls_cmd);
	return 0;
}


//    cat out && (in < ls |grep "c" | wc -l > out) || echo "hello world"
 

//    <in >out ls | wc -l >out2 2>out.err | cat out >out3 && echo coucou || cat out