/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 16:39:50 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 15:39:02 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_red(t_redirect *red)
{
	ft_lstfree(&(red->files), call_free_fs);
}

void	free_common(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_pwd(cmd->env);
	ht_del_hash_table(cmd->env);
	ft_lstfree((cmd->export_history), free);
	ft_lstfree(&(cmd->filenames), free);
}

void	rec_free_commands(t_cmd *cmd)
{
	t_cmd	*next;
	t_cmd	*red;

	if (!cmd)
		return ;
	next = cmd->next;
	red = cmd->red.next_cmd;
	rec_free_commands(next);
	rec_free_commands(red);
	free_cmd(&cmd);
}

void	free_cmds(t_cmd **cmd, bool common)
{
	if (!cmd || !*cmd)
		return ;
	if (common)
		free_common(*cmd);
	rec_free_commands(*cmd);
	*cmd = NULL;
}
