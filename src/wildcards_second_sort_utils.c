/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_second_sort_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:33:51 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ft_search_str_in_str() renvoie NULL en cas d'échec
char	*ft_search_str_in_str(char *s, char *to_find)
{
	int	i;
	int	j;

	if (!s || ft_strlen(to_find) > ft_strlen(s))
		return (NULL);
	if (!ft_strlen(to_find))
		return (s);
	i = -1;
	j = 0;
	while (s[++i])
	{
		if (s[i] == to_find[j])
		{
			while (s[i] && to_find[j] && s[i] == to_find[j])
			{
				i++;
				j++;
			}
			if (s[i] || (!s[i] && !to_find[j]))
				return (&s[i - j + 1]);
			i -= j;
			j = 0;
		}
	}
	return (NULL);
}

int	get_next_wcard_index(t_token_list *current, size_t index)
{
	int	i;

	i = index;
	while (current->content[i])
	{
		if (current->content[i] == '*' && !is_in_quotes(current, i))
			return (i);
		i++;
	}
	return (-1);
}

void	del_filename(t_filename **first, t_filename *to_del)
{
	t_filename	*current;

	current = (*first);
	if (*first == to_del)
	{
		*first = (*first)->next;
		free(to_del->filename);
		free(to_del);
		to_del = NULL;
		return ;
	}
	while (current->next && current->next != to_del)
		current = current->next;
	if (!current->next)
		return ;
	current->next = to_del->next;
	free(to_del->filename);
	free(to_del);
	to_del = NULL;
}
