/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:22:29 by Helene            #+#    #+#             */
/*   Updated: 2023/06/27 21:58:58 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_control_op(t_data *data, t_token_list **current)
{
	t_token_list	*curr;

	if (((*current))->length != 2)
		display_se(data, ft_substr(((*current))->content, 0, 2));
	curr = (*current)->next;
	while (curr && curr->type == whitespace)
		curr = curr->next;
	if (!curr)
		display_se(data, ft_strdup("newline"));
	if (curr->type == r_parenthesis)
		display_se(data, ft_strdup(")"));
	*current = (*current)->next;
}

void	check_pipe(t_data *data, t_token_list **current)
{
	t_token_list	*curr;

	/* while (*current && (*current)->type == whitespace)
			*current = (*current)->next; */
	curr = (*current)->next;
	while (curr && curr->type == whitespace)
		curr = curr->next;
	if (!(curr))
		display_se(data, ft_strdup("newline")); //display_se(data, ft_strdup("|"));
	if ((curr)->type == r_parenthesis)
		display_se(data, ft_strdup(")"));
	if ((curr)->type == and_tk)
		display_se(data, ft_substr((curr)->next->content, 0, 2));
	(*current) = (*current)->next; // ATTENTION VOIR SI CASSE PAS TOUT
}

void	check_pipelines(t_data *data, int *parentheses_count)
{
	t_token_list	*current;

	current = *(data->first);
	while (current)
	{
		if ((current->type == and_tk || (current->type) == or_tk)
			&& current->type > 1)
			check_control_op(data, &current);
		else
		{
			check_first(data, &current);
			while (current && current->type != and_tk && (current->type != or_tk
					|| current->length == 1))
			{
				if (current->type == or_tk && current->length == 1)
					check_pipe(data, &current);
				else
					check_simple_command(data, &current, parentheses_count);
			}
		}
	}
}
