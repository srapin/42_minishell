/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 21:44:19 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 01:09:55 by hlesny           ###   ########.fr       */
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
			// free and return. Quel exit status ?
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
			// free en cascade. Quel exit status ?
		}
		*pipeline_start_tk = (*current_tk)->next;
		return (1);
	}
	return (0);
}

t_cmd	*get_ast(t_data *data)
{
	t_token_list	*pipeline_start_tk;
	t_token_list	*current_tk;
	t_cmd			**ast;
	t_cmd			*pipeline_start_cmd;
	t_cmd			*current_cmd;

	t_cmd			*test; // est-ce que cette variable est utile ?
	ast = malloc(sizeof(t_cmd *));
	/* if (!ast && malloc_error(data,
				...)) (avec malloc_error qui retourne tjr 1, 
	comme ca economise 3 lignes ici)*/
	if (!ast)
	{
		malloc_error();
		return (NULL);
	}
	*ast = init_new_cmd(data);
	if (!*ast)
		return (NULL);
	pipeline_start_tk = *(data->first);
	pipeline_start_cmd = *ast;
	while (pipeline_start_tk)
	{
		current_tk = pipeline_start_tk;
		current_cmd = pipeline_start_cmd;
		/* tant que n'est ni un '&&' ni un '||', ie peut avoir des pipes,
			mais ne touche ici pas à la variable next de t_cmd */
		while (current_tk && current_tk->type != and_tk
			&& (current_tk->type != or_tk || current_tk->length == 1))
		{
			set_simple_command(current_cmd, data->first, &current_tk);
			set_pipe(data, &current_tk, &current_cmd);
		}
		if (!set_t_ctrl_op(data, &current_tk, &pipeline_start_tk,
				&pipeline_start_cmd))
		{
			current_cmd->ctrl = pointvirgule;
			pipeline_start_tk = NULL;
		}
	}
	test = *ast;
	free(ast);
	return (test);
}
