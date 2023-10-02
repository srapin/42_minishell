/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:31:24 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 13:58:17 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	valid_name(char *name)
{
	int	i;

	if (!name)
		return (-1);
	if (!name[0] || (name[0] && !ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && (name[i] != '-' && name[i] != '_'))
			return (0);
		i++;
	}
	return (1);
}

void	lst_del_first(t_list **export_hist, t_list *current)
{
	*export_hist = (*export_hist)->next;
	free(current->content);
	current->content = NULL;
	free(current);
	current = NULL;
}

/* Deletes var_name from export history. 
Does nothing if the export_history does not contain var_name*/
void	del_from_export_history(t_list **export_hist, char *var_name)
{
	t_list	*current;
	t_list	*current_next;

	current = *export_hist;
	if (!ft_strcmp(current->content, var_name))
		return (lst_del_first(export_hist, current));
	while (current->next)
	{
		if (!ft_strcmp(current->next->content, var_name))
		{
			current_next = current->next;
			current->next = current_next->next;
			free(current_next->content);
			current_next->content = NULL;
			free(current_next);
			current_next = NULL;
			return ;
		}
		current = current->next;
	}
}

int	is_in_export_history(t_list *export_hist, char *var_name)
{
	t_list	*current;

	current = export_hist;
	while (current)
	{
		if (!ft_strcmp(current->content, var_name))
			return (1);
		current = current->next;
	}
	return (0);
}
