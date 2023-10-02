/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_var_name.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:27:44 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_next_token(t_token_list **curr, size_t dollar_index)
{
	int				dollars_count;
	t_token_list	*current;

	(void) dollar_index;
	current = *curr;
	dollars_count = 0;
	while (current && (current->type == word || current->type == simple_quote
			|| current->type == double_quote)
		&& is_only_dollars(current->content))
	{
		if (current->next && (current->next->type == simple_quote
				|| current->next->type == double_quote))
		{
			dollars_count = ft_strlen(current->content);
			if (dollars_count && dollars_count % 2)
				remove_char(current, 0);
		}
		current = current->next;
	}
}

char	*set_var_name(t_token_list *current, size_t dollar_index,
		size_t next_dollar_index, int exp_case)
{
	int		k;
	char	*var_name;

	k = 1;
	if (exp_case == 1)
		k = 2;
	if (exp_case == 0)
		var_name = ft_substr(current->content, dollar_index + 1,
				next_dollar_index - dollar_index - 1);
	else
		var_name = ft_substr(current->content, dollar_index + 1, current->length
				- dollar_index - k);
	return (var_name);
}

/* Returns NULL in case no valid identifier follows the '$' 
at the given <dollar_index> index
Case 0 : is followed by another dollar sign (ie next_dollar_index != -1)
Case 1 : is inside double quotes
Case 2 : not followed by another dollar sign, and not in quotes */
char	*get_valid_id(t_token_list *current, size_t dollar_index,
		size_t next_dollar_index, int exp_case)
{
	int		k;
	char	*var_name;

	k = 1;
	if (exp_case == 1)
		k = 2;
	var_name = set_var_name(current, dollar_index, next_dollar_index, exp_case);
	while (k + dollar_index <= current->length && !valid_name(var_name))
	{
		free(var_name);
		k++;
		if (exp_case == 0)
			var_name = ft_substr(current->content, dollar_index + 1,
					next_dollar_index - dollar_index - k);
		else
			var_name = ft_substr(current->content, dollar_index + 1,
					current->length - dollar_index - k);
	}
	if (k + dollar_index <= current->length)
		return (var_name);
	free(var_name);
	return (NULL);
}

char	*get_var_name(t_token_list *current, char *next_d_start, size_t d_index,
		size_t next_d_index)
{
	char	*var_name;

	if (next_d_start && *next_d_start)
		var_name = get_valid_id(current, d_index, next_d_index, 0);
	else if (current->type == double_quote)
		var_name = get_valid_id(current, d_index, next_d_index, 1);
	else
		var_name = get_valid_id(current, d_index, next_d_index, 2);
	return (var_name);
}
