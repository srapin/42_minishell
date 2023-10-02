/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_quotes_group_words_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:14:44 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rm_quotes_token(t_token_list *current)
{
	char	*content_tmp;

	content_tmp = ft_substr(current->content, 1, current->length - 2);
	if (!content_tmp)
		return ;
	free(current->content);
	current->content = content_tmp;
	current->type = word;
	current->length -= 2;
}

t_word_data	*new_word_data(t_token_list *token)
{
	t_word_data	*wd;

	wd = ft_calloc(sizeof(t_word_data), 1);
	if (!wd)
		return (NULL);
	wd->content = ft_strdup(token->content);
	wd->quotes = token->quotes;
	wd->length = token->length;
	wd->next = NULL;
	return (wd);
}

void	add_word_data(t_word_data **first, t_word_data *to_add)
{
	size_t		word_end_index;
	t_word_data	*current;

	if (!first)
		return ;
	word_end_index = 0;
	if (!(*first))
		*first = to_add;
	else
	{
		current = (*first);
		while (current->next)
		{
			word_end_index += current->length;
			current = current->next;
		}
		word_end_index += current->length;
		to_add->word_start_index = word_end_index;
		current->next = to_add;
	}
}

void	del_whitespace(t_data *data, t_token_list **current)
{
	t_token_list	*current2;

	if (*current == *(data->first))
		*(data->first) = (*current)->next;
	current2 = *current;
	*current = (*current)->next;
	if (current2->prev)
		current2->prev->next = current2->next;
	if (current2->next)
		current2->next->prev = current2->prev;
	free(current2->content);
	free(current2);
}
