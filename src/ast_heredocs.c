/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_heredocs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 23:24:33 by Helene            #+#    #+#             */
/*   Updated: 2023/06/27 19:57:45 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

/* variable expansions,
	reusing functions used in the main parsing but without checking syntax 
Attention ! On expand la variable meme si elle est entre single quotes */
void	hd_perform_expand(t_ht_hash_table *ht, char **str)
{
	size_t			len;
	char			*tmp;
	char			*tmp2;
	t_token_list	*t_list;
	t_token_list	**first;

	if (!str || !(*str)[0])
		return ;
	len = ft_strlen(*str);
	first = tokenise(assign_type(*str, len), len, *str);
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
	free_tokens(first);
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
2) using readline(), rewrite the user's input in that file
3) expand variables if needed (ie if there were quotes in the limiter or not)
4) replace the initial tokens "<<" "LIM" by "<" "heredoc_file_path" in the token list   
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
		// return ?
	}
	quotes = set_quotes(current);
	get_here_doc_content(ht, fd, limiter, quotes);
	close(fd);
}

void	hd_sigint(int i)
{
	(void)i;
	g_exit_status = 130;
	// A VOIR PAS SUR
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	hd_child_process(t_data *data)
{
	t_token_list	*current;

	current = *(data->first);
	g_exit_status = 0;
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
