/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:08:36 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 15:38:54 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*init_new_cmd(t_data *data)
{
	t_cmd	*cmd;

	cmd = ft_calloc(sizeof(t_cmd), 1);
	if (!cmd)
	{
		perror("malloc ");
		return (NULL);
	}
	cmd->env = data->env;
	cmd->export_history = &data->exp_history;
	cmd->filenames = data->files;
	init_redirections(&(cmd->red));
	cmd->val.value = NULL;
	cmd->val.args = NULL;
	cmd->val.path = NULL;
	cmd->ctrl = pointvirgule;
	cmd->next = NULL;
	cmd->pid = -1;
	return (cmd);
}

void	add_to_cmd_list(t_cmd **last, t_cmd *new)
{
	if (!last)
		return ;
	if (!(*last))
		*last = new;
	else
	{
		(*last)->next = new;
		*last = new;
	}
}

/* Determine if the current token is a control operator, ie a "&&" or a "||" */
int	is_a_t_ctrl_op(t_token_list *current)
{
	if (current->type == and_tk)
		return (1);
	if (current->type == or_tk && current->length == 2)
		return (1);
	return (0);
}
