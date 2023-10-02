/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_struct_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:14:54 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_list	*tk_new_elem(char *t_stream, size_t t_len, int t_type,
		int is_quote)
{
	t_token_list	*new;

	new = ft_calloc(sizeof(t_token_list), 1);
	if (!new)
		return (NULL);
	new->content = ft_substr(t_stream, 0, t_len);
	new->length = t_len;
	new->type = t_type;
	new->quotes = is_quote;
	new->merged_words = NULL;
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

void	tk_add(t_token_list **first, t_token_list *new)
{
	t_token_list	*current;

	if (!first)
		return ;
	if (!*first)
		*first = new;
	else
	{
		current = *first;
		while (current->next)
			current = current->next;
		current->next = new;
		new->prev = current;
	}
}

void	tk_del_one(t_token_list **first, t_token_list *to_del)
{
	t_token_list	*current;

	if (!to_del)
		return ;
	current = *first;
	while (current && current != to_del)
		current = current->next;
	if (!current)
		return ;
	if (current->prev)
		current->prev->next = current->next;
	if (current->next)
		current->next->prev = current->prev;
	if (*first == current)
		*first = current->next;
	free(current->content);
	free(current);
	current = NULL;
}

/* adds a token <word> of content char *content after the *current */
void	tk_add_word_in_list(t_token_list **current, char *content)
{
	t_token_list	*initial_current_next;
	t_token_list	*new;

	initial_current_next = (*current)->next;
	new = tk_new_elem(content, ft_strlen(content), word, 0);
	(*current)->next = new;
	new->next = initial_current_next;
	if (initial_current_next)
		initial_current_next->prev = new;
	new->prev = (*current);
}
