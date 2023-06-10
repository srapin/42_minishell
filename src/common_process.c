/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:44:39 by srapin            #+#    #+#             */
/*   Updated: 2023/06/10 21:44:48 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


void	parent_process(t_cmd **cmd, int pipe_tab[2])
{
	char **test = hash_map_to_tab((*cmd)->env);
	
	safe_close(&(pipe_tab[1]));
	if (!cmd || !(*cmd))
		return;
	safe_close_cmd_fd(*cmd);
	//if ((*cmd)->red.in_type == fd && ((t_file *) ((*cmd)->red.in_content))->sep)
		//close
	*cmd = (*cmd)->red.next_cmd;
	//dprintf(STDOUT_FILENO, "parent pro %s\n", (*cmd)->val.value);
	if (!cmd || !(*cmd))
		return;
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
	dprintf(1, "in child proc outfile name = %s\n", ((t_file *)(cmd->red.out_list)->content)->name   );
	dup_cmd_file(cmd);
	// dprintf(1, "child_proc");
	paths = get_path(cmd);
	if (check_acces(paths, cmd))
	{
	 	execve(cmd->val.path, cmd->val.args, cmd->val.env);
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