/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:34:40 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_wildcard_opendir(char *error, char *dir_name)
{
	char	*mess;

	mess = ft_strjoin(error, dir_name);
	if (errno == ENOENT)
		g_exit_status = CMD_NOT_FOUND;
	else if (errno == 13)
		g_exit_status = NO_FILE_OR_DIR;
	else if (errno == EISDIR)
		g_exit_status = CMD_NOT_EXECUTABLE;
	else
		g_exit_status = NO_FILE_OR_DIR;
	perror(mess);
	free(mess);
	mess = NULL;
}

void	free_filenames(t_filename **filenames)
{
	t_filename	*curr;

	curr = *filenames;
	while (*filenames)
	{
		curr = *filenames;
		*filenames = (*filenames)->next;
		if (*filenames)
			curr->next = (*filenames)->next;
		free(curr->filename);
		free(curr);
	}
}

/*
Replaces current->content (ie the current token's content) 
with the associated files/directories names,	
in case the wildcard search revealed itself successful
*/
void	insert_filenames(t_token_list **first, t_token_list **current,
		t_filename **filenames)
{
	t_token_list	*tmp;
	t_filename		*current_f;

	if (!filenames || !(*filenames))
		return ;
	if (!(*filenames)->filename)
	{
		free(*filenames);
		return ;
	}
	current_f = (*filenames);
	tmp = (*current);
	while (current_f)
	{
		tk_add_word_in_list(current, current_f->filename);
		*current = (*current)->next;
		current_f = current_f->next;
	}
	tk_del_one(first, tmp);
	free_filenames(filenames);
}

void	free_and_null(char *prefix, char *suffix)
{
	free(prefix);
	free(suffix);
	prefix = NULL;
	suffix = NULL;
}

int	is_in_quotes(t_token_list *current, size_t index)
{
	size_t		length;
	t_word_data	*current_w;

	if (!(current->merged_words))
		return (current->quotes);
	current_w = current->merged_words;
	length = 0;
	while (current_w)
	{
		length += current_w->length;
		if (index >= current_w->word_start_index)
		{
			if ((!current_w->next && length > index)
				|| current_w->next->word_start_index > index)
				return (current_w->quotes);
		}
		current_w = current_w->next;
	}
	return (-1);
}
