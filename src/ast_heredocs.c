/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_heredocs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 23:24:33 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_here_doc(t_ht_hash_table *ht, t_token_list *current)
{
	char	*limiter;
	char	*file_name;
	int		quotes;
	int		fd;

	limiter = current->next->content;
	file_name = current->content;
	fd = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0600);
	if (fd == -1)
	{
		perror("open ");
		return ;
	}
	quotes = set_quotes(current);
	get_here_doc_content(ht, fd, limiter, quotes);
	close(fd);
}

void	hd_child_process(t_data *data)
{
	t_token_list	*current;

	current = *(data->first);
	g_exit_status = EXIT_OK;
	signal(SIGINT, hd_sigint);
	while (current && g_exit_status != 130)
	{
		if (current->type == l_io_redirect && current->length == 2)
		{
			current = current->next;
			set_here_doc(data->env, current);
		}
		current = current->next;
	}
	free_parsing_data(data);
	exit(g_exit_status);
}

bool	hd_parent_process(int pid, t_token_list *current)
{
	int	status;

	waitpid(pid, &status, 0);
	while (current)
	{
		if (current->type == l_io_redirect && current->length == 2)
		{
			current->length = 1;
			current = current->next;
			tk_del_one(&current, current->next);
		}
		current = current->next;
	}
	return (!(WIFEXITED(status) && WEXITSTATUS(status)));
}

/* Gives the prompt back to the user for here_doc contents, 
deleting the later from the tokens' list while doing so */
bool	set_here_docs(t_data *data)
{
	t_token_list	*first;
	int				pid;

	first = *(data->first);
	set_hd_filenames(first, data);
	pid = fork();
	if (pid < 0)
		fail_process();
	else if (pid == 0)
		hd_child_process(data);
	else
		return (hd_parent_process(pid, first));
	return (true);
}
