/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:13:44 by Helene            #+#    #+#             */
/*   Updated: 2023/06/21 04:27:11 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

/*
Après l'expansion de variables. 
Enleve les quotes de début et de fin, et remplace le token <quote> par <word>
*/
void	delete_quotes(t_data *data)
{
	char			*content_tmp;
	t_token_list	*current;

	if (!data->first || !(*(data->first)))
		return ;
	current = *(data->first);
	while (current)
	{
		if (current->type == l_parenthesis)
		{
			while (current && current->type != r_parenthesis)
				current = current->next;
			if (current)
				current = current->next;
		}
		else
		{
			if (current->type == simple_quote || current->type == double_quote)
				rm_quotes_token(current);
			current = current->next;
		}
	}
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

void	set_merged_words(t_token_list **curr)
{
	char			*content_tmp;
	t_token_list	*current;
	t_token_list	*tmp;

	current = *curr;
	if (!current->merged_words)
		add_word_data(&current->merged_words, new_word_data(current));
	add_word_data(&current->merged_words, new_word_data(current->next));
	content_tmp = ft_strjoin(current->content, current->next->content);
	free(current->content);
	current->content = content_tmp;
	current->length = ft_strlen(content_tmp);
	tmp = current->next;
	tmp->prev->next = tmp->next;
	if (tmp->next)
		tmp->next->prev = tmp->prev;
	free(tmp->content);
	free(tmp);
	tmp = NULL;
	*curr = current;
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

/*
Apres avoir enlevé les quotes, groupe deux <word> qui se suivent 
(comprendre ne sont pas séparés par des whitespaces,
		ou un opérateur de controle autre que des quotes)
-> il est alors obligatoire d'avoir gardé les tokens <whitespace> jusque là
*/
void	group_words(t_data *data)
{
	t_token_list	*current;
	char			*content_tmp;

	//t_token_list    *current2;
	if (!data->first)
		return ;
	current = *(data->first);
	while (current)
	{
		if (current->type == l_parenthesis)
		{
			while (current && current->type != r_parenthesis)
				current = current->next;
			if (current)
				current = current->next;
		}
		else if (current->type == word && current->next
				&& current->next->type == word)
			set_merged_words(&current);
		else if (current->type == whitespace)
			del_whitespace(data, &current);
		else
			current = current->next;
	}
}
