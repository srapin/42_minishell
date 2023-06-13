/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pub.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 23:56:51 by srapin            #+#    #+#             */
/*   Updated: 2023/06/13 23:33:11 by hlesny           ###   ########.fr       */
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

void	ft_lstfree(t_list **lst)
{
	t_list	*tmp;

	while (lst && *lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		free(tmp->content);
		ft_lstdelone(tmp);
	}
	lst = NULL;
}
//attention ne free pas l env
void free_cmd_value(t_cmd_value *val)
{
	free(val->value);
	free_tab(val->args);
	if (val->path)
	{
		val->path =NULL;	
		free(val->path);
	}
}

void free_cmd(t_cmd **cmd)
{
	
	*cmd = NULL;
}

void free_common(t_cmd*cmd)
{
	if (!cmd)
		return;
	
	
}

void rec_free_commands(t_cmd *cmd)
{
	t_cmd *next;
	t_cmd *red;
	
	next = cmd->next;
	red = cmd->red.next_cmd;
	if (!red && !next)
		return free_cmd(&cmd);
	rec_free_commands(next);
	rec_free_commands(red);
}

void free_commands(t_cmd *cmd)
{
	free_common(cmd);
	rec_free_commands(cmd);
}