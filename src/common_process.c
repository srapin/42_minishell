/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:44:39 by srapin            #+#    #+#             */
/*   Updated: 2023/06/19 04:17:51 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


void	parent_process(t_cmd **cmd, int pipe_tab[2])
{
	
	safe_close(&(pipe_tab[1]));
	if (!cmd || !(*cmd))
		return;
	safe_close_cmd_fd(*cmd);
	//if ((*cmd)->red.in_type == fd && ((t_file *) ((*cmd)->red.in_content))->sep)
		//close
	// t_cmd ** tmp;
	// tmp = cmd;
	if (!cmd || !(*cmd))
		return;
	(*cmd)->red.in_fd = pipe_tab[0];
}

void print_err(int e, t_cmd *cmd)
{
	
	// 2 : cmd not found. 13 : permission denied
	char *mess;
	char *tmp;

	mess = NULL;
	tmp = ft_strjoin(mess, "minishell : ");
	free(mess);
	mess = ft_strjoin(tmp, cmd->val.value);
	free(tmp);
	perror(mess);
	free(mess);
	// if (e == 2)
	// 	printf("minishell : %s : command not found\n", cmd->val.value);
	// else if (e == 13)
	// 	printf("minishell : %s : permission denied\n", cmd->val.value);
}

void	child_process(t_cmd *cmd, t_cmd *first,  int pipe_tab[2])
{
	char	**paths;
	char	*path;
	char ** args;
	char **env;
	char **tmp;

	//dprintf(1, "exec child proc \n");
	if (pipe_tab[0] > -1)
	{
		safe_close(&(pipe_tab[0]));
		cmd->red.out_fd = pipe_tab[1];
	}
	signal(SIGINT, SIG_DFL);
	dup_cmd_file(cmd);
	
	try_to_exec_builtins(cmd, first, true);
	
	paths = get_path(cmd);
	if (check_acces(paths, cmd))
	{
		path = cmd->val.path;
		env = cmd->val.env;
		args = cmd->val.args;
		cmd->val.args = NULL;
		cmd->val.path = NULL;
		cmd->val.env = NULL;
		free_tab(paths);
		free_cmds(&first, true);
	 	execve(path, args, env);
	}
	cmd->val.args = args;
	cmd->val.path = path;
	cmd->val.env = env;
	print_err(errno, cmd);
	free_tab(paths);
	free_cmds(&first, true);
	exit(CMD_NOT_FOUND);
}

void	fail_process(void)
{
	perror("fork error");
}