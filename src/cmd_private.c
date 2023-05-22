/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_private.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 23:55:31 by srapin            #+#    #+#             */
/*   Updated: 2023/05/23 01:55:34 by srapin           ###   ########.fr       */
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

void init_cmd(t_cmd *cmd, char **envp)
{
	add_value_to_cmd(cmd, NULL);
	add_args_to_cmd(cmd, NULL);
	init_redirections(&(cmd->red));
	cmd->env = envp;
	cmd->ctrl = none;	
	cmd->next = NULL;
}
void add_cmdval_to_cmd(t_cmd *cmd, char *str)
{
	add_args_to_cmd(cmd, str);
	if (cmd->args)
		add_value_to_cmd(cmd, cmd->args[0]);
}