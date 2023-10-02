/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_cmd_args_count.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:05:06 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	no_merged_words_count(t_token_list *current)
{
	int	count;
	int	wht_pos;
	int	p_wht_pos;

	count = 0;
	if (!current->quotes)
	{
		p_wht_pos = 0;
		wht_pos = get_whtsp_pos(current->content, p_wht_pos);
		while (wht_pos != -1)
		{
			if (wht_pos - p_wht_pos)
				count++;
			skip_whitespaces_reassess_indexes(current->content,
				&wht_pos, &p_wht_pos);
		}
		if ((size_t) p_wht_pos < current->length)
			count++;
	}
	else
		count++;
	return (count);
}

void	wd_no_quotes(char *content, int *buffer, int *count)
{
	int	wht_pos;
	int	p_wht_pos;

	p_wht_pos = 0;
	wht_pos = get_whtsp_pos(content, p_wht_pos);
	while (wht_pos != -1)
	{
		if (wht_pos - p_wht_pos)
		{
			(*count)++;
			*buffer = 0;
		}
		while (content[wht_pos] && (content[wht_pos] == ' '
				|| content[wht_pos] == '\t'))
			wht_pos++;
		p_wht_pos = wht_pos;
		wht_pos = get_whtsp_pos(content, p_wht_pos);
	}
	if (wht_pos == -1)
		*buffer = 1;
}

int	get_words_count(t_token_list *current)
{
	int			count;
	int			buffer;
	t_word_data	*wd;

	buffer = 0;
	count = 0;
	if (!current->content[0])
		return (1);
	wd = current->merged_words;
	if (!wd)
		return (no_merged_words_count(current));
	while (wd)
	{
		if (!wd->quotes)
			wd_no_quotes(wd->content, &buffer, &count);
		else
			buffer = 1;
		wd = wd->next;
	}
	if (buffer)
		count++;
	return (count);
}
