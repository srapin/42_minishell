/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_private.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 23:55:31 by srapin            #+#    #+#             */
/*   Updated: 2023/06/09 23:11:48 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"



void add_value_to_cmd(t_cmd *cmd, char *val) //do not use!
{
	// printf("3 add_value_to_cmd cmd = %p, val= %s\n", cmd, val);
	cmd->val.value = val;
}

void add_args_to_cmd(t_cmd *cmd, char *args)//do not use!
{
	// printf("2 add_args_to_cmd cmd = %p, args= %s\n", cmd, args);
	cmd->val.args = ft_split(args, ' ');
	// printf("2 add_args_to_cmd cmd = %p, args= %p\n", cmd, cmd->val.args);
}

void init_cmd(t_cmd *cmd, t_ht_hash_table *envp)
{
	add_value_to_cmd(cmd, NULL);
	add_args_to_cmd(cmd, NULL);
	init_redirections(&(cmd->red));
	cmd->env = envp;
	cmd->ctrl = none;	
	cmd->next = NULL;
	cmd->val.path = NULL;
	cmd->val.value = NULL;
	cmd->val.env = NULL;
}
void add_cmdval_to_cmd(t_cmd *cmd, char *str)
{
	// printf("1 add_cmdval, str=%s\n", str);
	add_args_to_cmd(cmd, str);
	if (cmd->val.args && *(cmd->val.args))
		add_value_to_cmd(cmd, ft_strdup((cmd->val.args)[0]));
}