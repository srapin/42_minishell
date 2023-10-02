/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_heredocs_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 06:10:47 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*random_filename(void)
{
	static size_t	files_count;
	char			*count;
	char			*filename;
	int				tmp_fd;

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
	tmp_fd = open(filename, O_CREAT, S_IRWXU);
	safe_close(&tmp_fd);
	free(count);
	return (filename);
}

int	set_quotes(t_token_list *current)
{
	int			quotes;
	t_word_data	*wd;

	quotes = 0;
	if (!current->next->merged_words)
		quotes = (current->next->quotes > 0);
	else
	{
		quotes = 0;
		wd = current->next->merged_words;
		while (wd && !quotes)
		{
			if (wd->quotes > 0)
				quotes = 1;
			wd = wd->next;
		}
	}
	return (quotes);
}

/*
1) create a temporary file (in /tmp)
2) using readline(), rewrite the user's input 
in that file
3) expand variables if needed (ie if there were 
   quotes in the limiter or not)
4) replace the initial tokens "<<" "LIM" by "<" "heredoc_file_path" 
   in the token list   
*/
void	set_hd_filenames(t_token_list *current, t_data *data)
{
	char	*file_name;

	while (current)
	{
		if (current->type == l_io_redirect && current->length == 2)
		{
			file_name = random_filename();
			ft_lstadd_back(&(data->files), ft_lstnew(file_name));
			tk_add_word_in_list(&current, file_name);
		}
		current = current->next;
	}
}
