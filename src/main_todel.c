/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:28:06 by srapin            #+#    #+#             */
/*   Updated: 2023/06/07 23:40:06 by Helene           ###   ########.fr       */
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
	cmd->red.next_cmd = next;
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
	
	add_value_to_cmd(cmd, "wc");
	add_args_to_cmd(cmd, "wc -l");
	// add_value_to_cmd(cmd, "cat");
	// add_args_to_cmd(cmd, "cat");
	init_file_struct_with_filename(f_s, "out");
	add_out_redir_with_file_struct(cmd, f_s);
	
}



void make_grep_cmd(t_cmd * cmd, t_cmd * wc_cmd, char ** envp)
{
	init_cmd(cmd, envp);
	
	add_value_to_cmd(cmd, "grep");
	add_args_to_cmd(cmd, "grep c");
	// add_value_to_cmd(cmd, "cat");
	// add_args_to_cmd(cmd, "cat");
	link_cmds_with_redirections(cmd, wc_cmd);
}

void make_ls_cmd(t_cmd * cmd, t_cmd * grep_cmd,t_file *f_s,  char ** envp)
{
	init_cmd(cmd, envp);
	
	add_value_to_cmd(cmd, "ls");
	add_args_to_cmd(cmd, "ls");
	link_cmds_with_redirections(cmd, grep_cmd);
	// init_file_struct_with_filename(f_s, "in");
	init_file_struct_with_sep(f_s, "in");
	// add_in_redir_with_file_struct(cmd, f_s);
}

void make_echo_salut_node(t_cmd *cmd, t_cmd *next_cmd, t_file *f_s,  char ** envp)
{

	char *str;

	str = ft_strdup("echo salut");
	init_cmd_and_add_val(cmd, envp, str);
	init_file_struct_with_filename(f_s, "out");
	add_out_redir_with_file_struct(cmd, f_s);
	link_cmds_with_ctrls_op(cmd, next_cmd, and);
}


void make_echo_youhou_node(t_cmd *cmd, char ** envp)
{

	init_cmd_and_add_val(cmd, envp, "echo youhou");
	// init_file_struct_with_filename(f_s, "out");
	// add_out_redir_with_file_struct(cmd, f_s);
	// link_cmds_with_ctrls_op(cmd, next_cmd, and);
}

void make_echo_hello_node(t_cmd *cmd, t_cmd *next_cmd, char ** envp)
{

	init_cmd_and_add_val(cmd, envp, "echo hello world");
	// init_file_struct_with_filename(f_s, "out");
	// add_out_redir_with_file_struct(cmd, f_s);
	link_cmds_with_ctrls_op(cmd, next_cmd, and);
}

void make_first_par_node(t_cmd *cmd, t_cmd *next_cmd, char ** envp)
{

	init_cmd_and_add_val(cmd, envp, "minishell 1");
	link_cmds_with_ctrls_op(cmd, next_cmd, or);
}

void make_second_par_node(t_cmd *cmd, char ** envp)
{

	init_cmd_and_add_val(cmd, envp, "minishell 2");
	// printf("coucou");
}

int main(int ac, char **av, char **envp)
{
	
	t_file cat_f;
	t_file wc_f;
	t_file ls_f;
	t_file echo_salut_fs;
	
	t_cmd cat_cmd;
	
	
	t_cmd echo_salut_cmd;
	
	t_cmd first_par_cmd;

	t_cmd ls_cmd;
	t_cmd grep_cmd;
	t_cmd wc_cmd;
	
	t_cmd seconde_par_cmd;
	
	t_cmd echo_world_cmd;
	t_cmd echo_youhou_cmd;

// echo salut > out || (ls |grep "c" | wc -l > out) && (echo "hello world" && echo "youhou")
	if (ft_atoi(av[1])== 0)
	{
		// printf("deb\n");
		make_echo_salut_node(&echo_salut_cmd, &first_par_cmd, &echo_salut_fs, envp);
		// printf("next echo\n");
		make_first_par_node(&first_par_cmd, &seconde_par_cmd, envp)	;
		// printf("next first\n");
		make_second_par_node(&seconde_par_cmd, envp);
		// printf("next second\n");
		exec_cmds(&echo_salut_cmd);
	}
	else if (ft_atoi(av[1]) == 1)
	{
		make_ls_cmd(&ls_cmd, &grep_cmd, &ls_f, envp);
		make_grep_cmd(&grep_cmd, &wc_cmd, envp);
		make_wc_cmd(&wc_cmd, &wc_f, envp);
		exec_cmds(&ls_cmd);
	}
	else
	{
		make_echo_hello_node(&echo_world_cmd, &echo_youhou_cmd, envp);
		make_echo_youhou_node(&echo_youhou_cmd, envp);
		exec_cmds(&echo_world_cmd);
	}


	//exec_cmds(&ls_cmd);
	return 0;
}



// ------------------------ 

//    cat out && (in < ls |grep "c" | wc -l > out) || echo "hello world"
 

//    <in >out ls | wc -l >out2 2>out.err | cat out >out3 && echo coucou || cat out
