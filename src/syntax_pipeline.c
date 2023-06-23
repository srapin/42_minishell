/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:22:29 by Helene            #+#    #+#             */
/*   Updated: 2023/06/23 15:22:50 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_control_op(t_data *data, t_token_list **current)
// ie "||" or "&&"
{
	t_token_list *curr;

	if (((*current))->length != 2)
		display_se(data, ft_substr(((*current))->content, 0, 2));
	curr = (*current)->next;
	while (curr && curr->type == whitespace)
		curr = curr->next;
	if (!curr) // faut-il redonner le prompt ?
		display_se(data, ft_strdup("newline"));
	// ou ft_substr((*current)->content, 0, 2) ?
	if (curr->type == r_parenthesis)
		display_se(data, ft_strdup(")"));

	*current = (*current)->next;
	// ou simplement *current = curr ? skip les whitespaces ici comme ca c'est fait
}

void	check_pipe(t_data *data, t_token_list **current)
{
	t_token_list	*curr;

	if (!(*current)->next) // faut-il redonner le prompt ?
		display_se(data, ft_strdup("|"));
	curr = (*current)->next;
	while (curr && curr->type == whitespace)
		curr = curr->next;
	if ((*current)->next->type == r_parenthesis)
		display_se(data, ft_strdup(")"));
	if ((*current)->next->type == and_tk)
		display_se(data, ft_substr((*current)->next->content, 0, 2));
	(*current) = (*current)->next;
}

void	check_pipelines(t_data *data, int *parentheses_count)
{
	t_token_list	*current;

	current = *(data->first);
	while (current)
	{
		if (current->type == and_tk || (current->type) == or_tk
			&& current->type > 1)
			check_control_op(data, &current);
		else
		{
			check_first(data, &current);
			/* check simple commands one by one */
			while (current && current->type != and_tk && (current->type != or_tk
					|| current->length == 1))
			{
				if (current->type == or_tk && current->length == 1)
					// ie si est un pipe
					check_pipe(data, &current);
				else
					check_simple_command(data, &current, parentheses_count);
			}
		}
	}
}
