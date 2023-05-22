/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:44:39 by srapin            #+#    #+#             */
/*   Updated: 2023/05/23 01:49:04 by srapin           ###   ########.fr       */
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
	if ((*cmd)->red.out_type == cmds_cmd)
		*cmd = (*cmd)->red.out_content;
	//dprintf(STDOUT_FILENO, "parent pro %s\n", (*cmd)->val.value);
	// if (!cmd || !(*cmd))
	// 	return;
	(*cmd)->red.in_fd = pipe_tab[0];
}

void	child_process(t_cmd *cmd, int pipe_tab[2], int *to_free)
{
	char	**paths;

	if (pipe_tab[0] > -1)
	{
		safe_close(&(pipe_tab[0]));
		cmd->red.out_fd = pipe_tab[1];
	}
	dup_cmd_file(cmd);
	paths = get_path(cmd->env);
	if (check_acces(paths, cmd))
	{
	 	execve(cmd->val.path, cmd->val.args, cmd->env);
	}
	perror("cmd not found");
	free_tab(paths);
	free_tab(cmd->val.args);
	free(to_free);
	exit(EXIT_FAILURE);
}

void	fail_process(void)
{
	perror("fork error");
}