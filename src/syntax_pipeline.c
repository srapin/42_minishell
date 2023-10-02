/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:22:29 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_control_op(t_data *data, t_token_list **current)
{
	t_token_list	*curr;

	if (((*current))->length != 2)
	{
		display_se(data, ft_substr(((*current))->content, 0, 2));
		return (SYNTAX_ERROR);
	}
	curr = (*current)->next;
	while (curr && curr->type == whitespace)
		curr = curr->next;
	if (!curr)
	{
		display_se(data, ft_strdup("newline"));
		return (SYNTAX_ERROR);
	}
	if (curr->type == r_parenthesis)
	{
		display_se(data, ft_strdup(")"));
		return (SYNTAX_ERROR);
	}
	*current = (*current)->next;
	return (0);
}

int	check_pipe(t_data *data, t_token_list **current)
{
	t_token_list	*curr;

	curr = (*current)->next;
	while (curr && curr->type == whitespace)
		curr = curr->next;
	while (curr && curr->type == whitespace)
		curr = curr->next;
	if (!(curr))
	{
		display_se(data, ft_strdup("newline"));
		return (SYNTAX_ERROR);
	}
	if ((curr)->type == r_parenthesis)
	{
		display_se(data, ft_strdup(")"));
		return (SYNTAX_ERROR);
	}
	if ((curr)->type == and_tk)
	{
		display_se(data, ft_substr((curr)->content, 0, 2));
		return (SYNTAX_ERROR);
	}
	(*current) = (*current)->next;
	return (0);
}

int	check_pipelines(t_data *data, int *parentheses_count)
{
	t_token_list	*current;

	current = *(data->first);
	while (current)
	{
		if ((current->type == and_tk || (current->type) == or_tk)
			&& current->type > 1 && check_control_op(data, &current))
			return (SYNTAX_ERROR);
		else
		{
			if (check_first(data, &current))
				return (SYNTAX_ERROR);
			while (current && current->type != and_tk && (current->type != or_tk
					|| current->length == 1))
			{
				if (current->type == or_tk && current->length == 1
					&& check_pipe(data, &current))
					return (SYNTAX_ERROR);
				else if (check_simple_command(data,
						&current, parentheses_count))
					return (SYNTAX_ERROR);
			}
		}
	}
	return (0);
}
