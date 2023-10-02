/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:12:06 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_exit_status(t_token_list *current,
	char **d_start, char *next_d_start)
{
	if (is_exit_status(current, *d_start))
	{
		*d_start = next_d_start;
		return (1);
	}
	return (0);
}

void	get_expanded_token(t_ht_hash_table *ht,
		t_token_list **current, char *d_start)
{
	char			*next_d_start;
	char			*var_name;
	size_t			next_d_index;
	size_t			d_index;

	while (d_start && *d_start)
	{
		d_index = (*current)->length - ft_strlen(d_start);
		next_d_start = ft_strdup(ft_strchr(d_start + 1, '$'));
		next_d_index = (*current)->length - ft_strlen(next_d_start);
		if (check_for_exit_status(*current, &d_start, next_d_start))
			continue ;
		var_name = get_var_name(*current, next_d_start, d_index, next_d_index);
		if (var_name)
			search_and_expand(ht, current, var_name, d_index);
		else
			check_next_token(current, d_index);
		free(d_start);
		d_start = next_d_start;
		free(var_name);
	}
	free(d_start);
	d_start = NULL;
}

void	parse_current_tk(t_ht_hash_table *ht, t_token_list **first,
		t_token_list **current)
{
	char			*d_start;

	(void)first;
	d_start = ft_strdup(ft_strchr((*current)->content, '$'));
	get_expanded_token(ht, current, d_start);
	check_for_empty_content(first, current);
}

void	perform_variable_exp(t_data *data)
{
	t_token_list	*current;

	current = *(data->first);
	while (current)
	{
		while (current && current->type == whitespace)
			current = current->next;
		if (!current)
			break ;
		if (current->type == l_parenthesis)
		{
			while (current && current->type != r_parenthesis)
				current = current->next;
			if (current)
				current = current->next;
			continue ;
		}
		else if (current->type != simple_quote && (!current->prev
				|| current->prev->type != l_io_redirect
				|| current->prev->length == 1))
			parse_current_tk(data->env, data->first, &current);
		else
			current = current->next;
	}
}
