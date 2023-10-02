/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_quotes_group_words.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:13:44 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Après l'expansion de variables. 
Enleve les quotes de début et de fin, et remplace le token <quote> par <word>
*/
void	delete_quotes(t_data *data)
{
	t_token_list	*current;

	if (!data->first || !(*(data->first)))
		return ;
	current = *(data->first);
	while (current)
	{
		if (!current->content)
			current = current->next;
		else if (current->type == l_parenthesis)
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

/*
Apres avoir enlevé les quotes, groupe deux <word> qui se suivent 
(comprendre ne sont pas séparés par des whitespaces,
		ou un opérateur de controle autre que des quotes)
-> il est alors obligatoire d'avoir gardé les tokens <whitespace> jusque là
*/
void	group_words(t_data *data)
{
	t_token_list	*current;

	if (!data->first)
		return ;
	current = *(data->first);
	while (current)
	{
		if (!current->content)
			current = current->next;
		else if (current->type == l_parenthesis)
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
