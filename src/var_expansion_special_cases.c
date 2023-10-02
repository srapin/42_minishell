/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_special_cases.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 20:38:59 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_exit_status(t_token_list *current, char *d_start)
{
	int		i;
	char	*value;
	char	*after_value;
	char	*before_key;
	size_t	dollar_index;

	i = 1;
	if (!(*d_start))
		return (0);
	if (d_start[i] && d_start[i] == '?')
	{
		dollar_index = current->length - ft_strlen(d_start + i) - 1;
		before_key = ft_substr(current->content, 0, dollar_index);
		value = ft_itoa(g_exit_status);
		after_value = ft_substr(current->content, dollar_index + 2,
				current->length);
		update_tk_content(&current, before_key, value, after_value);
		check_next_token(&current, current->length - ft_strlen(d_start) - 1);
		free(d_start);
		free(value);
		return (1);
	}
	return (0);
}

void	check_for_empty_content(t_token_list **first, t_token_list **current)
{
	t_token_list	*tmp;

	if ((*current) && !(*current)->content)
	{
		tmp = *current;
		(*current) = (*current)->next;
		tk_del_one(first, tmp);
	}
	else
		*current = (*current)->next;
}
