/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_simple_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:17:34 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 01:32:57 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_io_redirect(t_data *data, t_token_list **op)
{
	t_token_list	*current;

	if ((*op)->length > 2)
		display_se(data, ft_substr((*op)->content, 2, 2));
	current = (*op)->next;
	while (current && current->type == whitespace)
		current = current->next;
	if (!current)
	{
		display_se(data, ft_strdup("newline"));
		return (SYNTAX_ERROR);
	}
	else if (current->type != word && current->type != simple_quote
		&& current->type != double_quote)
	{
		display_se(data, ft_substr(current->content, 0, 2));
		return (SYNTAX_ERROR);
	}
	return (0);
}

int	sc_check_quotes(t_data *data, t_token_list **current)
{
	if ((*current)->type == simple_quote
		&& ((*current)->content[(*current)->length - 1] != '\''
			|| (*current)->length == 1))
	{
		printf("Syntax error : Missing closing quote\n");
		free_parsing_data(data);
		return (SYNTAX_ERROR);
		//exit(SYNTAX_ERROR);
	}
	else if ((*current)->type == double_quote
		&& ((*current)->content[(*current)->length - 1] != '\"'
			|| (*current)->length == 1))
	{
		printf("Syntax error : Missing closing quote\n");
		free_parsing_data(data);
		return (SYNTAX_ERROR);
		//exit(SYNTAX_ERROR);
	}
	return (0);
}

int	sc_check_parentheses(t_data *data, t_token_list **current,
		int *parentheses_count)
{
	if ((*current)->type == l_parenthesis)
	{
		t_token_list	*curr;

		curr = (*current)->next;
		while (curr && curr->type == whitespace)
			curr = curr->next;
		printf("est ici\n");
		(*parentheses_count)++;
		/* *current = (*current)->next;
		while (*current && (*current)->type == whitespace)
			*current = (*current)->next; */
		if (curr && (curr)->type == r_parenthesis)
		{
			printf("ici 1\n");
			display_se(data, ft_strdup(")"));
			return (SYNTAX_ERROR);
		}
		else if (curr && (curr)->type == l_parenthesis)
			(*parentheses_count)++;
		else if (!(curr))
		{
			printf("Syntax error : Missing closing parenthesis\n");
			free_parsing_data(data);
			return (SYNTAX_ERROR);
			//exit(SYNTAX_ERROR);
		}
		//(*current) = (*current)->next;
	}
	else if ((*current)->type == r_parenthesis)
	{
		printf("est la\n");
		if (!(*parentheses_count))
		{
			printf("ici 2\n");
			display_se(data, ft_strdup(")"));
			return (SYNTAX_ERROR);
		}
		(*parentheses_count)--;
	}
	return (0);
}

int	check_simple_command(t_data *data, t_token_list **current,
		int *parentheses_count)
{
	while ((*current) && (*current)->type != and_tk
		&& (*current)->type != or_tk)
	{
		if (((*current)->type == l_io_redirect
			|| (*current)->type == r_io_redirect)
			&& check_io_redirect(data, current))
			return (SYNTAX_ERROR);
		else if (((*current)->type == l_parenthesis
			|| (*current)->type == r_parenthesis)
			&& sc_check_parentheses(data, current, parentheses_count))
			return (SYNTAX_ERROR);
		else if (((*current)->type == simple_quote
			|| (*current)->type == double_quote)
			&& sc_check_quotes(data, current))
			return (SYNTAX_ERROR);
		*current = (*current)->next;
	}
	return (0);
}
