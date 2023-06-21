/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 04:36:57 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/21 04:37:10 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
Prints the syntax error while specifying the token
Then,
	frees all data structures and returns the corresponding exit status (ie 2 for syntax error)
*/
void	display_se(t_data *data, char *token)
{
	printf("Syntax error near unexpected token `%s\'\n", token);
	free_parsing_data(data);
	/* free_tokens(first); */
	free(token);
	exit(SYNTAX_ERROR);
}

void	check_first(t_data *data, t_token_list **first)
{
	int				type;
	t_token_list	*current;

	if (!first || !*first)
		exit(EXIT_OK); // rien a free dans ce cas, si ?
	current = (*first);
	while (current && current->type == whitespace)
		current = current->next;
	if (!current)
		return ;
	type = current->type;
	if (type == and_tk)
	{
		if ((*first)->length >= 2)
			display_se(data, ft_strdup("&&"));
		display_se(data, ft_strdup("&"));
	}
	else if (type == or_tk)
	{
		if ((*first)->length >= 2)
			display_se(data, ft_strdup("||"));
		display_se(data, ft_strdup("|"));
	}
	else if (type == r_parenthesis)
		display_se(data, ft_strdup(")"));
}

/* Les tokens avec des '' ou "" doivent avoir le type word,
	et le content "\0" !!! */
void	check_io_redirect(t_data *data, t_token_list **op)
{
	t_token_list	*current;

	if ((*op)->length > 2)
		display_se(data, ft_substr((*op)->content, 2, 2));
	// garbage collector ? permettrait d'y ajouter le malloc de ft_substr et reglerait le probleme de ou peut le free
	current = (*op)->next;
	while (current && current->type == whitespace)
		current = current->next;
	if (!current)
		// cas ou rien ne suit : ie syntax error near unexpected newline
		display_se(data, ft_strdup("newline"));
	else if (current->type != word && current->type != simple_quote
			&& current->type != double_quote)
		display_se(data, ft_substr(current->content, 0, 2));
}

void	sc_check_quotes(t_data *data, t_token_list **current)
{
	if ((*current)->type == simple_quote
		&& ((*current)->content[(*current)->length - 1] != '\''
			|| (*current)->length == 1))
	{
		printf("Syntax error : Missing closing quote\n");
		free_parsing_data(data);
		exit(SYNTAX_ERROR);
	}
	else if ((*current)->type == double_quote
			&& ((*current)->content[(*current)->length - 1] != '\"'
				|| (*current)->length == 1))
	{
		printf("Syntax error : Missing closing quote\n");
		free_parsing_data(data);
		exit(SYNTAX_ERROR);
	}
}

void	sc_check_parentheses(t_data *data, t_token_list **current,
		int *parentheses_count)
{
	if ((*current)->type == l_parenthesis)
	{
		(*parentheses_count)++; // c'est tout ?
		if ((*current)->next && (*current)->next->type == r_parenthesis)
			display_se(data, ")");
	}
	else if ((*current)->type == r_parenthesis)
	{
		if (!(*parentheses_count))
			display_se(data, ")");
		(*parentheses_count)--;
	}
}

void	check_simple_command(t_data *data, t_token_list **current,
		int *parentheses_count)
{
	while ((*current) && (*current)->type != and_tk
		&& (*current)->type != or_tk)
	{
		if ((*current)->type == l_io_redirect
			|| (*current)->type == r_io_redirect)
			check_io_redirect(data, current);
		else if ((*current)->type == l_parenthesis
				|| (*current)->type == r_parenthesis)
			sc_check_parentheses(data, current, parentheses_count);
		else if ((*current)->type == simple_quote
				|| (*current)->type == double_quote)
			sc_check_quotes(data, current);
		*current = (*current)->next;
	}
}

void	check_control_op(t_data *data, t_token_list **current)
// ie "||" or "&&"
{
	t_token_list *curr;

	if (((*current))->length != 2)
		display_se(data, ft_substr(((*current))->content, 0, 2));
	curr = (*current)->next;
	while (curr && curr->type == whitespace)
		curr = curr->next;
	if (!curr) // faut-il redonner le prompt ?
		display_se(data, ft_strdup("newline"));
	// ou ft_substr((*current)->content, 0, 2) ?
	if (curr->type == r_parenthesis)
		display_se(data, ft_strdup(")"));

	*current = (*current)->next;
	// ou simplement *current = curr ? skip les whitespaces ici comme ca c'est fait
}

void	check_pipe(t_data *data, t_token_list **current)
{
	t_token_list	*curr;

	if (!(*current)->next) // faut-il redonner le prompt ?
		display_se(data, ft_strdup("|"));
	curr = (*current)->next;
	while (curr && curr->type == whitespace)
		curr = curr->next;
	if ((*current)->next->type == r_parenthesis)
		display_se(data, ft_strdup(")"));
	if ((*current)->next->type == and_tk)
		display_se(data, ft_substr((*current)->next->content, 0, 2));
	(*current) = (*current)->next;
}

void	check_pipelines(t_data *data, int *parentheses_count)
{
	t_token_list	*current;

	current = *(data->first);
	while (current)
	{
		if (current->type == and_tk || (current->type) == or_tk
			&& current->type > 1)
			check_control_op(data, &current);
		else
		{
			check_first(data, &current);
			/* check simple commands one by one */
			while (current && current->type != and_tk && (current->type != or_tk
					|| current->length == 1))
			{
				if (current->type == or_tk && current->length == 1)
					// ie si est un pipe
					check_pipe(data, &current);
				else
					check_simple_command(data, &current, parentheses_count);
			}
		}
	}
}

void	check_syntax(t_data *data)
{
	int	parentheses_count;

	dprintf(1, "syntax\n");
	parentheses_count = 0;
	if (!data || !(data->first) || !(*(data->first)))
		return ;
	check_first(data, data->first);
	/* check pipelines one by one */
	check_pipelines(data, &parentheses_count);
	if (parentheses_count)
	{
		printf("Syntax error : Missing closing parenthesis\n");
		free_parsing_data(data);
		exit(SYNTAX_ERROR);
	}
	free_parsing_data(data);
	exit(EXIT_SUCCESS);
}

int	ft_syntax(t_data *data)
{
	pid_t	pid;
	int		wstatus;

	pid = fork();
	if (pid == -1)
		perror("fork ");
	if (pid == 0) // ie s'agit du process enfant
		check_syntax(data);
	if (waitpid(pid, &wstatus, 0) == -1)
		perror("wait ");
	return (wstatus);
}
