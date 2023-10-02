/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 21:44:19 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	malloc_error(void)
{
}

void	set_pipe(t_data *data, t_token_list **current_tk, t_cmd **current_cmd)
{
	if (*current_tk && (*current_tk)->type == or_tk
		&& (*current_tk)->length == 1)
	{
		(*current_cmd)->red.next_cmd = init_new_cmd(data);
		if (!(*current_cmd)->red.next_cmd)
		{
			perror("malloc ");
			return ;
		}
		*current_cmd = (*current_cmd)->red.next_cmd;
		*current_tk = (*current_tk)->next;
	}
}

int	set_t_ctrl_op(t_data *data, t_token_list **current_tk,
		t_token_list **pipeline_start_tk, t_cmd **pipeline_start_cmd)
{
	if (*current_tk && is_a_t_ctrl_op(*current_tk))
	{
		(*pipeline_start_cmd)->ctrl = ((*current_tk)->type == and_tk)
			* and + ((*current_tk)->type == or_tk) * or ;
		(*pipeline_start_cmd)->next = init_new_cmd(data);
		*pipeline_start_cmd = (*pipeline_start_cmd)->next;
		if (!(*pipeline_start_cmd))
		{
			perror("malloc ");
			*pipeline_start_tk = NULL;
			return (0);
		}
		*pipeline_start_tk = (*current_tk)->next;
		return (1);
	}
	*pipeline_start_tk = NULL;
	return (0);
}

t_cmd	*get_ast(t_data *data)
{
	t_token_list	*p_start_tk;
	t_token_list	*curr_tk;
	t_cmd			*ast;
	t_cmd			*p_start_cmd;
	t_cmd			*current_cmd;

	ast = init_new_cmd(data);
	if (!ast)
		return (NULL);
	p_start_tk = *(data->first);
	p_start_cmd = ast;
	while (p_start_tk)
	{
		curr_tk = p_start_tk;
		current_cmd = p_start_cmd;
		while (curr_tk && curr_tk->type != and_tk
			&& (curr_tk->type != or_tk || curr_tk->length == 1))
		{
			set_simple_command(current_cmd, &curr_tk);
			set_pipe(data, &curr_tk, &current_cmd);
		}
		if (!set_t_ctrl_op(data, &curr_tk, &p_start_tk, &p_start_cmd))
				current_cmd->ctrl = pointvirgule;
	}
	return (ast);
}
