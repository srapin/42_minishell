/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_heredocs_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 04:31:02 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 04:39:54 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*random_filename(void)
{
	static size_t	files_count;
	char			*count;
	char			*filename;

	count = ft_itoa(files_count);
	filename = ft_strjoin("/tmp/here_doc_tmp_", count);
	while (access(filename, F_OK) == 0)
	{
		free(filename);
		free(count);
		files_count++;
		count = ft_itoa(files_count);
		filename = ft_strjoin("/tmp/here_doc_tmp_", count);
	}
	free(count);
	return (filename);
}

int	is_limiter(char *line, char *limiter)
{
	int	i;

	i = 0;
	while (line[i] && limiter[i] && line[i] == limiter[i])
		i++;
	if (!limiter[i])
		return (1);
	return (0);
}



void	hd_expand(t_ht_hash_table *ht, t_token_list **t_list)
{
	char			*next_dollar_start;
	char			*dollar_start;
	size_t			next_dollar_index;
	size_t			dollar_index;
	t_token_list	*current;
	int				k;
	char			*var_name;

	current = *t_list;
	while (current)
	{
		while (current && current->type == whitespace)
			current = current->next;
		if (!current)
			break ;
		dollar_start = ft_strdup(ft_strchr(current->content, '$'));
		while (dollar_start && *dollar_start)
		{
			dollar_index = current->length - ft_strlen(dollar_start);
			next_dollar_start = ft_strdup(ft_strchr(dollar_start + 1, '$'));
			next_dollar_index = current->length - ft_strlen(next_dollar_start);
			if (next_dollar_start && *next_dollar_start)
				expand(ht, &current, ft_substr(current->content, dollar_index
					+ 1, next_dollar_index - dollar_index - 1),
					dollar_index);
			else if (current->type == double_quote
					|| current->type == simple_quote)
			{
				k = 2;
				var_name = ft_substr(current->content, dollar_index + 1,
						current->length - dollar_index - k);
				while (k + dollar_index <= current->length
					&& !valid_name(var_name))
				{
					free(var_name);
					k++;
					var_name = ft_substr(current->content, dollar_index + 1,
							current->length - dollar_index - k);
				}
				if (k + dollar_index <= current->length)
					expand(ht, &current, var_name, dollar_index);
			}
			else
				expand(ht, &current, ft_strdup(dollar_start + 1), dollar_index);
			free(dollar_start);
			dollar_start = NULL;
			dollar_start = next_dollar_start;
		}
		free(dollar_start);
		current = current->next;
	}
}