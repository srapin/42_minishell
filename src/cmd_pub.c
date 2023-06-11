/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pub.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 23:56:51 by srapin            #+#    #+#             */
/*   Updated: 2023/06/07 22:54:12 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/priv.h"
#include "../inc/minishell.h"

void add_val_to_cmd(t_cmd *cmd, char *args)
{
	char *test = ft_strdup(args);
	add_cmdval_to_cmd(cmd, test);
} 

void init_cmd_and_add_val(t_cmd *cmd, char **envp, char *args)
{
	char *test = ft_strdup(args);
	init_cmd(cmd, envp);
	add_cmdval_to_cmd(cmd, test);
}

t_cmd *create_cmd_with_val(char **envp, char *args)
{
	t_cmd *cmd;

	cmd = malloc(sizeof(t_cmd));
	init_cmd_and_add_val(cmd, envp, args);
	return cmd;
}

int count_cmds(t_cmd *first_cmd)
{
	int nb;
	t_cmd *cmd;

	cmd = first_cmd;

	nb = 1;
	while(cmd->red.next_cmd)
	{
		nb++;
		cmd = cmd->red.next_cmd;
	}
	//printf("%d\n",nb);
	return nb;
}

char *get_cmd_value(t_cmd *cmd)
{
	return (cmd->val.value);
}

void dup_cmd_file(t_cmd *cmd)
{
	open_cmd_files(cmd);
	if (cmd->red.in_fd > -1)
	{
		dup2(cmd->red.in_fd, STDIN_FILENO);
		safe_close(&(cmd->red.in_fd));
	}
	if (cmd->red.out_fd > -1)
	{

		dup2(cmd->red.out_fd, STDOUT_FILENO);
		safe_close(&(cmd->red.out_fd));
	}
}