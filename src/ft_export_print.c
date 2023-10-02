/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:29:45 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	insert_in_sorted_lst(t_list **head, t_list *elem)
{
	t_list	*lst;

	if (!head || !elem)
		return ;
	lst = *head;
	if (!lst)
	{
		*head = elem;
		return ;
	}
	if (ft_strcmp((char *)lst->content, (char *)elem->content) >= 0)
		return (ft_lstadd_front(head, elem));
	search_and_insert(head, elem);
}

t_list	*get_sorted_hist(t_ht_hash_table *ht)
{
	char	*to_insert;
	t_list	*lst;
	t_list	*current;
	size_t	i;

	i = 0;
	lst = NULL;
	while (i < ht->size)
	{
		while (i < ht->size && (!ht->items[i] || (!ht->items[i]->key
					|| !ht->items[i]->value)))
			i++;
		if (!(i < ht->size) || (!ft_strcmp("_", ht->items[i]->key)))
			break ;
		to_insert = ht->items[i]->key;
		current = ft_lstnew(to_insert);
		insert_in_sorted_lst(&lst, current);
		i++;
	}
	return (lst);
}

bool	ft_lst_in(t_list *lst, char *str)
{
	while (lst)
	{
		if (ft_strisequal((char *)lst->content, str))
			return (true);
		lst = lst->next;
	}
	return (false);
}

void	print_sorted_hist(t_ht_hash_table *ht, t_list *export_hist, t_list *lst)
{
	t_list	*current_var;
	char	*str;

	current_var = lst;
	while (current_var)
	{
		str = ht_search(ht, current_var->content);
		if (!str && is_in_export_history(export_hist,
				(char *)current_var->content))
			printf("export %s\n", (char *)current_var->content);
		else if (str)
			printf("export %s=\"%s\"\n", (char *)current_var->content, str);
		current_var = current_var->next;
	}
}

// print toutes les variables ayant été exportées, triées par ordre alphabétique
void	print_export_history(t_ht_hash_table *ht, t_list *export_hist)
{
	t_list	*lst;
	t_list	*exp;
	t_list	*exp_next;

	lst = get_sorted_hist(ht);
	exp = export_hist;
	while (exp)
	{
		exp_next = exp->next;
		insert_in_sorted_lst(&lst, ft_lstnew(exp->content));
		exp = exp_next;
	}
	print_sorted_hist(ht, export_hist, lst);
	ft_lstclear(&lst);
}
