/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_cmd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:06:08 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Returns the substring starting at the first encountered 
whitespace in the initial string.
When no whitespace is encountered, NULL is returned.
If the string is either empty or null, NULL is returned. */
int	get_whtsp_pos(char *str, int whtsp_pos)
{
	int	i;

	if (!str)
		return (-1);
	i = whtsp_pos;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t')
			return (i);
		i++;
	}
	return (-1);
}

void	set_buffer(char **buffer, char *substr)
{
	char	*tmp;

	if (!buffer)
		return ;
	tmp = *buffer;
	*buffer = ft_strjoin(tmp, substr);
	free(substr);
	free(tmp);
	substr = NULL;
	tmp = NULL;
}

void	reassess_buffer(t_cmd **curr_cmd, char **buffer, int *i)
{
	if (!buffer)
		return ;
	if (*buffer && **buffer)
	{
		(*curr_cmd)->val.args[*i] = ft_strdup(*buffer);
		(*i)++;
		free(*buffer);
		*buffer = NULL;
	}
}

void	skip_whitespaces_reassess_indexes(char *curr_content, int *wht_pos,
		int *p_wht_pos)
{
	while (curr_content[*wht_pos] && (curr_content[*wht_pos] == ' '
			|| curr_content[*wht_pos] == '\t'))
		(*wht_pos)++;
	*p_wht_pos = *wht_pos;
	*wht_pos = get_whtsp_pos(curr_content, *p_wht_pos);
}

void	whitespaces_skip_assess(t_word_data *wd, int *wht_pos, int *p_wht_pos)
{
	while (wd->content[*wht_pos] && (wd->content[*wht_pos] == ' '
			|| wd->content[*wht_pos] == '\t'))
		(*wht_pos)++;
	*p_wht_pos = *wht_pos;
	*wht_pos = get_whtsp_pos(wd->content,
			*p_wht_pos);
}
