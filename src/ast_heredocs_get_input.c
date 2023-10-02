/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_heredocs_get_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 06:11:55 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_limiter(char *line, char *limiter)
{
	return (limiter && ft_strisequal(line, limiter));
}

void	hd_get_expanded_token(t_ht_hash_table *ht,
		t_token_list **current)
{
	char			*next_d_start;
	char			*var_name;
	char			*d_start;
	size_t			next_d_index;
	size_t			d_index;

	d_start = ft_strdup(ft_strchr((*current)->content, '$'));
	while (d_start && *d_start)
	{
		d_index = (*current)->length - ft_strlen(d_start);
		next_d_start = ft_strdup(ft_strchr(d_start + 1, '$'));
		next_d_index = (*current)->length - ft_strlen(next_d_start);
		if (check_for_exit_status(*current, &d_start, next_d_start))
			continue ;
		var_name = get_var_name(*current, next_d_start, d_index, next_d_index);
		if (var_name)
			search_and_expand(ht, current, var_name, d_index);
		free(d_start);
		d_start = next_d_start;
		free(var_name);
	}
	free(d_start);
	d_start = NULL;
}

void	hd_expand(t_ht_hash_table *ht, t_token_list **t_list)
{
	t_token_list	*current;

	current = *(t_list);
	while (current)
	{
		while (current && current->type == whitespace)
			current = current->next;
		if (!current)
			break ;
		hd_get_expanded_token(ht, &current);
		check_for_empty_content(t_list, &current);
	}
}

/* variable expansions,
	reusing functions used in the main parsing but without checking syntax 
Attention ! On expand la variable meme si elle est entre single quotes */
void	hd_perform_expand(t_ht_hash_table *ht, char **str)
{
	char			*tmp;
	char			*tmp2;
	t_token_list	*t_list;
	t_token_list	**first;

	if (!str || !(*str)[0])
		return ;
	first = tokenise(assign_type(*str, ft_strlen(*str)), ft_strlen(*str), *str);
	t_list = *first;
	hd_expand(ht, &t_list);
	tmp = NULL;
	while (t_list)
	{
		tmp2 = ft_strjoin(tmp, t_list->content);
		if (tmp)
			free(tmp);
		tmp = tmp2;
		t_list = t_list->next;
	}
	free(*str);
	*str = tmp;
	if (tmp)
		free_tokens(first);
	else
		free(first);
}

void	get_here_doc_content(t_ht_hash_table *ht, int fd, char *limiter,
		int quotes)
{
	char	*hd_content;

	hd_content = readline("> ");
	while (hd_content && !is_limiter(hd_content, limiter))
	{
		if (!quotes)
			hd_perform_expand(ht, &hd_content);
		if (write(fd, hd_content, ft_strlen(hd_content)) == -1)
			perror("write ");
		if (write(fd, "\n", 1) == -1)
			perror("write ");
		free(hd_content);
		hd_content = readline("> ");
	}
}
