/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:28:06 by srapin            #+#    #+#             */
/*   Updated: 2023/05/23 02:18:24 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/priv.h"

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