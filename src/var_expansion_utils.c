/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:26:43 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_tk_content(t_token_list **current, char *before_key, char *value,
		char *after_value)
{
	char			*tmp;

	free((*current)->content);
	tmp = ft_strjoin(before_key, value);
	(*current)->content = ft_strjoin(tmp, after_value);
	(*current)->length = ft_strlen((*current)->content);
	if (!(*(*current)->content))
	{
		free((*current)->content);
		(*current)->content = NULL;
	}
	free(before_key);
	free(after_value);
	free(tmp);
	before_key = NULL;
	after_value = NULL;
	tmp = NULL;
}

char	*get_next_d_start(char *d_start)
{
	int		i;
	char	*next_d_start;

	i = 0;
	while (d_start[i] && d_start[i] == '$')
		i++;
	next_d_start = ft_strdup(ft_strchr(d_start + i, '$'));
	return (next_d_start);
}

int	is_only_dollars(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '$')
			return (0);
		i++;
	}
	return (1);
}

void	remove_char(t_token_list *current, size_t index)
{
	char	*before;
	char	*after;

	before = ft_substr(current->content, 0, index);
	after = ft_substr(current->content, index + 1, current->length);
	if (!before || !after)
	{
		perror("malloc ");
		return ;
	}
	free(current->content);
	current->content = ft_strjoin(before, after);
	current->length--;
	free(before);
	free(after);
}
