/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 18:49:56 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_file_struct(t_file *file_struct)
{
	if (file_struct->name)
		free(file_struct->name);
	free(file_struct);
}

void	call_free_fs(void *p)
{
	t_file	*file;

	file = (t_file *)p;
	free_file_struct(file);
}

void	ft_lstfree(t_list **lst, void free_foo(void *))
{
	t_list	*tmp;

	while (lst && *lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		if (free_foo)
			free_foo(tmp->content);
		ft_lstdelone(tmp);
	}
	*lst = NULL;
}

void	free_cmd_value(t_cmd_value *val)
{
	free(val->value);
	free_tab(val->args);
	if (val->path)
	{
		val->path = NULL;
		free(val->path);
	}
	val->value = NULL;
	val->args = NULL;
	val->path = NULL;
}

void	free_cmd(t_cmd **p)
{
	t_cmd	*cmd;

	if (!p)
		return ;
	cmd = *p;
	if (!cmd)
	{
		*p = NULL;
		return ;
	}
	free_cmd_value(&(cmd->val));
	free_red(&(cmd->red));
	free(cmd);
	*p = NULL;
}
