/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_heredocs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 23:24:33 by Helene            #+#    #+#             */
/*   Updated: 2023/06/21 06:41:42 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*random_filename()
{
	static size_t	files_count;
	char			*count;
	char			*filename;

	count = ft_itoa(files_count);
	filename = ft_strjoin("/tmp/here_doc_tmp_", count);
	while (access(filename, F_OK | R_OK | W_OK) == 0)
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
	if (!limiter[i]) // ie a parcouru tout le limiter
		return (1);  // est le limiter
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
				char *var_name;

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
				if (k + dollar_index <= current->length) // ie valid_name() == 1
					expand(ht, &current, var_name, dollar_index);
			}
			else
				expand(ht, &current, ft_strdup(dollar_start + 1), dollar_index);
			////dprintf(1, "fin de while, ok ici\n");
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
void	hd_perform_expand(t_ht_hash_table *ht, char **str) // char* ou char ** ?
{
	size_t len;
	char *tmp;
	char *tmp2;
	t_token_list *t_list;
	t_token_list **first;

	if (!str || !(*str)[0])
		// str existe forcément (sinon serait sorti du while (readline()))
		return ;
	len = ft_strlen(*str);
	first = tokenise(assign_type(*str, len), len, *str);
	t_list = *first;
	hd_expand(ht, &t_list);

	/* Join all the tokens' contents*/
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
		if (!quotes) // if quotes in limiter,don't expand user's input
			hd_perform_expand(ht, &hd_content);
		// vérifier que hd_content est bien modifié après l'appel !
		if (write(fd, hd_content, ft_strlen(hd_content)) == -1)
			perror("write "); // return ? jpense pas
		if (write(fd, "\n", 1) == -1)
			// non ? car readline() retire le \n de fin
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

// void	set_hd_filenames(t_ht_hash_table *ht, t_token_list *current)
void	set_hd_filenames(t_token_list *current)
{
	char	*file_name;

	while (current)
	{
		if (current->type == l_io_redirect && current->length == 2)
		{
			file_name = random_filename();
			// current->length = 1;
			// free(current->next->content);
			tk_add_word_in_list(&current, file_name);
			free(file_name);
		}
		// ie est un here doc
		// current->next->content = file_name;
		// current->next->length = ft_strlen(file_name);
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
	fd = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0600); //  | O_EXCL
	// ////dprintf(1, "in set hd, %s, %s %i\n", limiter, file_name, fd);
	if (fd == -1)
	{
		perror("open ");
		// return ?
	}
	/* Sets the quotes variable,
		which indicates whether or not to expand the user's input (aka the heredoc content)*/
	quotes = set_quotes(current);
	get_here_doc_content(ht, fd, limiter, quotes);
	/* Give prompt back to user and write the heredoc content to the file*/
	close(fd);
}

void	hd_sigint(int i)
{
	// ////dprintf(1, "sig press \n");
	g_exit_status = 130;
	close(0);
	close(1);
	close(2);
}

void	hd_child_process(t_data *data)
{
	t_token_list	*current;

	//t_token_list *first;
	//first = current;
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
	/* free_pwd(ht);
	ht_del_hash_table(ht);
	free_tokens(&first);
	ft_lstfree(&exp_hist, free); */
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
	////dprintf(1, "%i\n", status);
	return (!(WIFEXITED(status) && WEXITSTATUS(status)));
	// return (true);
}

/* Gives the prompt back to the user for here_doc contents, 
deleting the later from the tokens' list while doing so */
bool	set_here_docs(t_data *data)
{
	t_token_list	*first;
	int				pid;
	int				status;

	//t_token_list	*current;
	//current = *first;
	first = *(data->first);
	// set_hd_filenames(ht, current);
	set_hd_filenames(first);
	// ////dprintf(1, "heyyyy\n");
	pid = fork();
	if (pid < 0)
		fail_process();
	else if (pid == 0)
	{
		//dprintf(1, "hd child proc \n");
		// hd_child_process(ht, first, exp_hist);
		hd_child_process(data);
	}
	else
	{
		return (hd_parent_process(pid, first));
		// wait(&status);
	}
	return (true);
}
